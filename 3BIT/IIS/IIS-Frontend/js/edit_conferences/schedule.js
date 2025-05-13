import { baseUrl } from '../../config/config.js';

function getConferenceId() {
    const urlParams = new URLSearchParams(window.location.search);
    return urlParams.get('conference_id');
}

let presentationsUnconfirmed = [];
let auditoriums = [];
let conference = {};
let presentationsConfirmed = [];

export function loadScheduleSection() {
    const scheduleSection = document.getElementById('schedule');
    scheduleSection.innerHTML = `
        <div id="schedule-container" class="row">
            <!-- Unscheduled Presentations -->
            <div class="col-md-4">
                <h3>Unscheduled</h3>
                <div id="unscheduled-presentations" class="border p-3" style="height: 300px; overflow-y: auto;">
                    <!-- Unscheduled presentations -->
                </div>
                <div class="mt-3">
                    <h4>Available Rooms</h4>
                    <div id="room-selection" class="form-check">
                        <!-- Checkboxes for selecting up to 5 rooms -->
                    </div>
                </div>
            </div>
            <div class="col-md-8">
                <h3>Schedule</h3>
                <div id="schedule-table-container" class="border">
                    <!--Schedule table -->
                </div>
                <button id="save-schedule-btn" class="btn btn-success mt-3">Save Schedule</button>
            </div>
        </div>
    `;

    const conferenceId = getConferenceId();

    Promise.all([
        fetch(`${baseUrl}/api/conferences/${conferenceId}/presentations/unconfirmed`)
            .then(res => res.json())
            .then(data => presentationsUnconfirmed = data),
        fetch(`${baseUrl}/api/conferences/${conferenceId}/auditoriums`)
            .then(res => res.json())
            .then(data => auditoriums = data),
        fetch(`${baseUrl}/api/conferences/${conferenceId}`)
            .then(response => response.json())
            .then(data => conference = data),
        fetch(`${baseUrl}/api/conferences/${conferenceId}/presentations/confirmed`)
            .then(response => response.json())
            .then(data => presentationsConfirmed = data)
    ])
        .then(() => {
            renderUnscheduledPresentations(presentationsUnconfirmed);
            loadLinkedAuditoriums(auditoriums, conference);
            renderScheduleTable(auditoriums.filter((_, idx) => idx < 4), conference);
            loadConfirmedPresentations(presentationsConfirmed, conference);
        })
        .catch(err => console.error("Error loading schedule data:", err));

    //Saving
    const saveScheduleBtn = document.getElementById('save-schedule-btn');
    saveScheduleBtn.addEventListener('click', () => {
        const changedCards = document.querySelectorAll('.changed-card');
        const unscheduledContainer = document.getElementById('unscheduled-presentations');
    
        const updates = [];
    
        changedCards.forEach(card => {
            const presentationId = card.dataset.id;
            const dropzone = card.closest('.dropzone');
            const update = { id: presentationId };
    
            if (unscheduledContainer.contains(card)) {
                update.start_time = null;
                update.end_time = null;
                update.status = 'unconfirmed';
            } else if (dropzone) {
                const timeSlot = dropzone.parentElement.firstChild.textContent;
                const auditoriumIndex = Array.from(dropzone.parentElement.children).indexOf(dropzone) - 1;
                const auditorium = auditoriums[auditoriumIndex];
    
                if (auditorium) {
                    const startTime = new Date(conference.start_time);
                    startTime.setHours(timeSlot.split(':')[0]);
                    const endTime = new Date(startTime.getTime() + 60 * 60 * 1000);

                    update.start_time = startTime.toISOString();
                    update.end_time = endTime.toISOString();
                    update.status = 'confirmed';
                    update.auditorium_id = auditorium.auditorium.id;
                }
            }
    
            updates.push(update);
        });

        Promise.all(updates.map(update => {
            return fetch(`${baseUrl}/api/presentations/${update.id}`, {
                method: 'PUT',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(update)
            })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Failed to save schedule');
                }
                return response.json();
            })
        }))
        .then(() => {
            alert('Schedule saved successfully!');
            changedCards.forEach(card => card.classList.remove('changed-card'));
        })
        .catch(err => console.error("Error saving schedule:", err));
    });
    
}

function loadConfirmedPresentations(confirmedPresentations, conference) {
    const scheduleTable = document.getElementById('schedule-table');

    if (!scheduleTable) {
        console.error("Schedule table not found!");
        return;
    }

    confirmedPresentations.forEach(presentation => {
        const { id, name, description, start_time, end_time, auditorium_id } = presentation;

        const startTime = new Date(start_time).toTimeString().slice(0, 5);
        const timeRow = Array.from(scheduleTable.rows).find(row => row.cells[0]?.textContent === startTime);

        if (!timeRow) {
            console.warn(`No matching time slot for presentation ${name}`);
            return;
        }

        const auditoriumIndex = auditoriums.findIndex(aud => aud.auditorium.id === auditorium_id);
        if (auditoriumIndex === -1) {
            console.warn(`No matching auditorium for presentation ${name}`);
            return;
        }

        const dropzone = timeRow.cells[auditoriumIndex + 1];
        if (!dropzone) {
            console.warn(`No dropzone found for presentation ${name}`);
            return;
        }

        const card = document.createElement('div');
        card.className = 'presentation-card';
        card.draggable = true;
        card.dataset.id = id;
        card.innerHTML = `
            <h5>${name}</h5>
            <p>${description}</p>
        `;

        dropzone.appendChild(card);
    });

    enableDragAndDrop();
}


function loadLinkedAuditoriums(auditoriums, conference) {
    const roomSelectionContainer = document.getElementById('room-selection');

    if (!auditoriums || auditoriums.length === 0) {
        roomSelectionContainer.innerHTML = `
            <p class="text-muted">No linked rooms for this conferention.</p>
        `;
        return;
    }

    roomSelectionContainer.innerHTML = auditoriums.map((aud_conf, index) => `
        <div class="form-check">
            <input 
                type="checkbox" 
                class="form-check-input room-checkbox blocked-checkbox" 
                id="room-${aud_conf.auditorium_id}" 
                value="${aud_conf.auditorium_id}" 
                checked/>
            <label class="form-check-label" for="room-${aud_conf.auditorium_id}">
                ${aud_conf.auditorium.name}
            </label>
        </div>
    `).join('');

    handleRoomSelectionChange(auditoriums, conference);
}

function handleRoomSelectionChange(auditoriums, conference) {
    const roomCheckboxes = document.querySelectorAll('.room-checkbox');

    roomCheckboxes.forEach(checkbox => {
        checkbox.addEventListener('change', () => {
            const checkedCheckboxes = Array.from(roomCheckboxes).filter(cb => cb.checked);
            
            if (checkedCheckboxes.length > 4) {
                checkbox.checked = false;
                alert("You can select up to 4 rooms only.");
                return;
            }

            const selectedRoomIds = checkedCheckboxes.map(cb => parseInt(cb.value));
            const selectedAuditoriums = auditoriums.filter(aud => selectedRoomIds.includes(aud.auditorium_id));
            renderScheduleTable(selectedAuditoriums, conference);
        });
    });
}


function generateTimeSlots(conference) {
    const startTime = new Date(conference.start_time);
    const endTime = new Date(conference.end_time);
    const slots = [];
    let current = startTime;

    while (current < endTime) {
        slots.push(current.toTimeString().slice(0, 5));
        current = new Date(current.getTime() + 60 * 60 * 1000);
    }
    return slots;
}


function renderUnscheduledPresentations(presentations) {
    const unscheduledPresentationsContainer = document.getElementById('unscheduled-presentations');

    if (!presentations || presentations.length === 0) {
        unscheduledPresentationsContainer.innerHTML = `
            <p class="text-muted">There are no presentations available for scheduling at this time.</p>
        `;
        return;
    }

    unscheduledPresentationsContainer.innerHTML = presentations.map(presentation => `
        <div class="presentation-card" draggable="true" data-id="${presentation.id}">
            <h5>${presentation.name}</h5>
            <p>${presentation.description}</p>
        </div>
    `).join('');

    enableDragAndDrop();
}

function renderScheduleTable(auditoriums, conference) {
    const scheduleTableContainer = document.getElementById('schedule-table-container');
    if (!auditoriums || auditoriums.length === 0) {
        scheduleTableContainer.innerHTML = `
            <p class="text-muted">To display the schedule, link rooms to the conference in the tab <strong>Auditoriums</strong>.</p>
        `;
        document.getElementById('save-schedule-btn').style.display = "none";
        return;
    }

    const timeSlots = generateTimeSlots(conference);
    const table = document.createElement('table');
    table.id = "schedule-table";

    // Header row
    const headerRow = document.createElement('tr');
    headerRow.innerHTML = `<th>Time</th>` + auditoriums.map(aud_conf => `<th>${aud_conf.auditorium.name}</th>`).join('');
    table.appendChild(headerRow);

    // Time rows
    timeSlots.forEach(slot => {
        const row = document.createElement('tr');
        row.innerHTML = `<td>${slot}</td>` + auditoriums.map(() => `<td class="dropzone"></td>`).join('');
        table.appendChild(row);
    });

    scheduleTableContainer.innerHTML = '';
    scheduleTableContainer.appendChild(table);

    enableDragAndDrop();
}

function enableDragAndDrop() {
    const cards = document.querySelectorAll('.presentation-card');
    const unscheduledContainer = document.getElementById('unscheduled-presentations');
    const dropzones = document.querySelectorAll('.dropzone');

    cards.forEach(card => {
        card.addEventListener('dragstart', event => {
            event.dataTransfer.setData('text/plain', card.dataset.id);
        });
    });

    dropzones.forEach(zone => {
        zone.addEventListener('dragover', event => {
            event.preventDefault();
            if (!zone.querySelector('.presentation-card')) {
                zone.classList.add('dragover');
            }
        });

        zone.addEventListener('dragleave', () => {
            zone.classList.remove('dragover');
        });

        zone.addEventListener('drop', event => {
            event.preventDefault();
            const presentationId = event.dataTransfer.getData('text/plain');
            const card = document.querySelector(`.presentation-card[data-id="${presentationId}"]`);

            if (card && !zone.querySelector('.presentation-card')) {
                zone.appendChild(card);
                card.classList.add('changed-card');
                zone.classList.remove('dragover');
            }
        });
    });

    unscheduledContainer.addEventListener('dragover', event => {
        event.preventDefault();
        unscheduledContainer.classList.add('dragover');
    });

    unscheduledContainer.addEventListener('dragleave', () => {
        unscheduledContainer.classList.remove('dragover');
    });

    unscheduledContainer.addEventListener('drop', event => {
        event.preventDefault();
        const presentationId = event.dataTransfer.getData('text/plain');
        const card = document.querySelector(`.presentation-card[data-id="${presentationId}"]`);
        if (card && !unscheduledContainer.contains(card)) {
            unscheduledContainer.appendChild(card);
            card.classList.add('changed-card'); // Mark card as changed
            unscheduledContainer.classList.remove('dragover');
        }
    });
}
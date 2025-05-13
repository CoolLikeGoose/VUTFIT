import { baseUrl } from '../../config/config.js';
import { loadScheduleSection } from './schedule.js'

function getConferenceId() {
    const urlParams = new URLSearchParams(window.location.search);
    return urlParams.get('conference_id');
}

function loadAvailableAuditoriums(conferenceId) {
    fetch(`${baseUrl}/api/conferences/${conferenceId}/auditoriums/available`)
        .then(response => response.json())
        .then(auditoriums => {
            const dropdown = document.getElementById('available-auditoriums');
            dropdown.innerHTML = '<option value="" disabled selected>Select auditorium</option>';
            auditoriums.forEach(auditorium => {
                const option = document.createElement('option');
                option.value = auditorium.id;
                option.textContent = auditorium.name;
                dropdown.appendChild(option);
            });
        })
        .catch(error => console.error('Error loading available auditoriums:', error));
}

export function loadAuditoriumsSection() {
    const auditoriumsSection = document.getElementById('auditoriums');
    auditoriumsSection.innerHTML = `
        <h2>Auditoriums</h2>
        <div class="d-flex">
            <select id="available-auditoriums" class="form-control mr-2">
                <option value="" disabled selected>Select auditorium</option>
            </select>
            <button class="btn btn-primary" id="add-auditorium-btn">Add</button>
        </div>
        <div id="auditoriums-container"></div>
    `;

    const conferenceId = getConferenceId();
    if (conferenceId) {
        loadAvailableAuditoriums(conferenceId);
        loadAuditoriums(conferenceId);
    }

    document.getElementById('add-auditorium-btn').addEventListener('click', () => {
        const auditoriumId = document.getElementById('available-auditoriums').value;
        if (!auditoriumId) return alert('Please select an auditorium.');
    
        const conferenceId = getConferenceId();
        fetch(`${baseUrl}/api/conferences/${conferenceId}/auditoriums/${auditoriumId}`, { method: 'POST' })
            .then(() => {
                loadAuditoriums(conferenceId);
                loadAvailableAuditoriums(conferenceId);
                loadScheduleSection();
            })
            .catch(error => console.error('Error adding auditorium:', error));
    });
    
}

function loadAuditoriums(conferenceId) {
    fetch(`${baseUrl}/api/conferences/${conferenceId}/auditoriums`)
        .then(response => response.json())
        .then(auditoriums_conferences => {
            const listContainer = document.getElementById('auditoriums-container');
            listContainer.innerHTML = auditoriums_conferences.map(auditorium_conf =>`
                <div class="d-flex justify-content-between align-items-center border p-2 mb-2">
                    <span>${auditorium_conf.auditorium.name}</span>
                    <button class="btn btn-danger btn-sm" onclick="removeAuditorium(${conferenceId}, ${auditorium_conf.auditorium_id})">Remove</button>
                </div>
            `).join('');
        })
        .catch(error => console.error('Error loading auditoriums:', error));
}

window.removeAuditorium = function (conferenceId, auditoriumId) {
    fetch(`${baseUrl}/api/conferences/${conferenceId}/auditoriums/${auditoriumId}`, { method: 'DELETE' })
        .then(() => {
            loadAuditoriums(conferenceId);
            loadScheduleSection();
        })
        .catch(error => console.error('Error removing auditorium:', error));
}

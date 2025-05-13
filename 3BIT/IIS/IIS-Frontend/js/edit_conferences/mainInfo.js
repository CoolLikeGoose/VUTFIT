import { baseUrl } from '../../config/config.js';

const user = JSON.parse(localStorage.getItem('user'));
function getConferenceId() {
  const urlParams = new URLSearchParams(window.location.search);
  return urlParams.get('conference_id');
}

function formatDate(timeString) {
  if (!timeString) return '';
  const date = new Date(timeString);
  const year = date.getFullYear();
  const month = String(date.getMonth() + 1).padStart(2, '0');
  const day = String(date.getDate()).padStart(2, '0');
  return `${year}-${month}-${day}`;
}


function formatTime(timeString) {
  if (!timeString) return '';
  const date = new Date(timeString);
  const hours = String(date.getHours()).padStart(2, '0');
  return `${hours}:00`;
}

export function loadMainInfoSection() {
    const mainInfoSection = document.getElementById('main-information');
    const conferenceId = getConferenceId();
    mainInfoSection.innerHTML = `
        <h2>Main Information</h2>
        <form id="main-info-form">
            <div class="form-group">
            <label for="conference-name">Name <imp class="important">*</imp></label>
            <input type="text" class="form-control" id="conference-name" placeholder="Enter conference name" required>
          </div>
          <div class="form-group">
            <label for="conference-description">Description <imp class="important">*</imp></label>
            <textarea class="form-control" id="conference-description" rows="3" required></textarea>
          </div>
          <div class="form-group">
            <label for="conference-capacity">Capacity <imp class="important">*</imp></label>
            <input type="number" class="form-control" id="conference-capacity" required>
          </div>
          <div class="form-group">
            <label for="conference-cost">Cost <imp class="important">*</imp></label>
            <input type="number" class="form-control" id="conference-cost" required>
          </div>
          <div class="form-group">
            <label for="max-reservations">Max Reservations Per User <imp class="important">*</imp></label>
            <input type="number" class="form-control" id="max-reservations" value="5" required>
          </div>
          <div class="form-group">
            <label for="conference-picture">Picture</label>
            <input type="text" class="form-control" id="conference-picture">
          </div>
          <div class="form-group">
            <label for="conference-tags">Tags</label>
            <input type="text" class="form-control" id="conference-tags" placeholder="Comma-separated tags">
          </div>
          <div class="row presentation-time-selection">
            <div class="form-group">
              <label for="conference-date">Presentation Date <imp class="important">*</imp></label>
              <input type="date" class="form-control" id="conference-date" required>
            </div>
            <div class="form-group">
              <label for="conference-time-start">Conference start <imp class="important">*</imp></label>
              <input type="time" class="form-control" id="conference-time-start" step="3600" required>
            </div>
            <div class="form-group">
              <label for="conference-time-end">Conference end <imp class="important">*</imp></label>
              <input type="time" class="form-control" id="conference-time-end" step="3600" required>
            </div>
          </div>
          <div class="nice-container">
            <button type="submit" class="btn btn-primary">Save</button>
            ${conferenceId ? `
              <button id="delete-conference-btn" class="btn btn-danger mt-3">Delete Conference</button>
            ` : ''}
          </div>
        </form>
    `;

    if (conferenceId) {
        fetch(`${baseUrl}/api/conferences/${conferenceId}`)
            .then(response => response.json())
            .then(conference => populateMainInfoForm(conference))
            .catch(() => alert('Failed to load conference data.'));
    }

    document.getElementById('main-info-form').addEventListener('submit', async (e) => {
        e.preventDefault();
        await saveMainInfo(conferenceId);
    });

    //handle delete btn
    const deleteBtn = document.getElementById('delete-conference-btn');
    if (deleteBtn) {
      deleteBtn.addEventListener('click', () => {
        if (confirm('Are you sure you want to delete this conference?')) {
            fetch(`${baseUrl}/api/conferences/${conferenceId}`, {
                method: 'DELETE',
                headers: { 'Content-Type': 'application/json' }
            })
                .then(response => {
                    if (response.ok) {
                        alert('Conference deleted successfully!');
                        window.location.href = '/pages/index.html';
                    } else {
                        throw new Error('Failed to delete the conference.');
                    }
                })
                .catch(error => {
                    console.error('Error deleting conference:', error);
                    alert('An error occurred while trying to delete the conference.');
                });
        }
    });
    }

    //restriction for time selection
    const dateInput = document.getElementById('conference-date');
    const timeStartInput = document.getElementById('conference-time-start');
    const timeEndInput = document.getElementById('conference-time-end');

    if (!dateInput.value) {
      const today = new Date();
      const tomorrow = new Date(today.getFullYear(), today.getMonth(), today.getDate() + 2);
      dateInput.min = tomorrow.toISOString().split('T')[0];
    }
}

function populateMainInfoForm(conference) {
    document.getElementById('conference-name').value = conference.name;
    document.getElementById('conference-description').value = conference.description || '';
    document.getElementById('conference-capacity').value = conference.capacity;
    document.getElementById('conference-cost').value = conference.cost;
    document.getElementById('max-reservations').value = conference.max_reservations_per_user;
    document.getElementById('conference-picture').value = conference.picture || '';

    document.getElementById('conference-date').value = formatDate(conference.start_time);
    document.getElementById('conference-time-start').value = formatTime(conference.start_time);
    document.getElementById('conference-time-end').value = formatTime(conference.end_time);

    const tags = conference.tags || [];
    document.getElementById('conference-tags').value = tags.join(', ');
}

async function saveMainInfo(conferenceId) {
    const name = document.getElementById('conference-name').value;
    const description = document.getElementById('conference-description').value;
    const capacity = document.getElementById('conference-capacity').value;
    const cost = document.getElementById('conference-cost').value;
    const maxReservations = document.getElementById('max-reservations').value;
    const picture = document.getElementById('conference-picture').value;

    const rawStartTime = document.getElementById('conference-time-start').value
    const rawEndTime = document.getElementById('conference-time-end').value
    if (rawStartTime >= rawEndTime) {
      alert("Please check conference start and end time");
      return;
    }
      
    const day = document.getElementById('conference-date').value;
    const startTime = `${day}T${rawStartTime}`;
    const endTime = `${day}T${rawEndTime}`;

    const tagsInput  = document.getElementById('conference-tags').value;
    const tags = tagsInput.split(',').map(tag => tag.trim()).filter(tag => tag);
    if (tags.length > 5) {
      alert("You can only add up to 5 tags.");
      return;
    }

    const data = {
        organizer_id: user.id,
        name: name,
        description,
        capacity,
        cost,
        max_reservations_per_user: maxReservations,
        current_occupancy: 0,
        picture,
        start_time: new Date(startTime),
        end_time: new Date(endTime),
        tags,
    };

    const method = conferenceId ? 'PUT' : 'POST';
    const endpoint = conferenceId ? `${baseUrl}/api/conferences/${conferenceId}` : `${baseUrl}/api/conferences`;

    const response = await fetch(endpoint, {
        method,
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(data),
    });

    if (response.ok) {
        alert('Conference saved successfully!');
        if (!conferenceId) {
            const newConference = await response.json();
            window.location.href = `/pages/edit_conference.html?conference_id=${newConference.id}`;
        }
    } else {
        alert('Failed to save conference.');
    }
}
import { baseUrl } from "../config/config.js";

// Check user authentication
const user = JSON.parse(localStorage.getItem('user'));

document.addEventListener("DOMContentLoaded", () => {
    if (!user) {
        alert("You need to be logged in to view this page.");
        window.location.href = "/pages/index.html";
        return;
    }

    // Event listeners for search and filters
    document.getElementById('create-conference-btn').addEventListener('click', () => {
        window.location.href = '/pages/edit_conference.html';
    });

    document.getElementById('search-input').addEventListener('input', loadData);
    document.getElementById('filter-type').addEventListener('change', loadData);
    document.getElementById('filter-time').addEventListener('change', loadData);

    loadData();
});

let currentPage = 0;

document.getElementById('next-page-btn').addEventListener('click', () => {
    currentPage++;
    loadData();
});

document.getElementById('prev-page-btn').addEventListener('click', () => {
    if (currentPage > 0) {
        currentPage--;
        loadData();
    }
});

// Load and filter data
function loadData() {
    const searchQuery = document.getElementById('search-input').value;
    const filterType = document.getElementById('filter-type').value;
    const filterTime = document.getElementById('filter-time').value;

    const dataContainer = document.getElementById('timetable-body');
    const currentPageDisplay = document.getElementById('current-page');
    dataContainer.innerHTML = '<tr><td colspan="4" class="text-center">Loading...</td></tr>';

    // Use the unified endpoint to fetch events
    const apiUrl = `${baseUrl}/api/users/${user.id}/events?event_type=${filterType}&search=${searchQuery}&filter_time=${filterTime}&start=${currentPage * 5}&limit=${5}`;

    fetch(apiUrl)
        .then(res => res.json())
        .then(responseData => {
            console.log(responseData);

            // Check if responseData contains events array
            if (!responseData.events || !Array.isArray(responseData.events)) {
                throw new Error("Unexpected response format");
            }

            renderData(responseData.events);

            // Update the page number display
            currentPageDisplay.textContent = `Page: ${currentPage + 1}`;

            // Disable/Enable pagination buttons based on data
            document.getElementById('prev-page-btn').disabled = currentPage === 0;
            document.getElementById('next-page-btn').disabled = responseData.events.length < 5;
        })
        .catch(error => {
            console.error('Error loading data:', error);
            dataContainer.innerHTML = '<tr><td colspan="4" class="text-center text-danger">Error loading data. Please try again later.</td></tr>';
        });
}

// Render data in a timetable format
function renderData(events) {
    const dataContainer = document.getElementById('timetable-body');
    dataContainer.innerHTML = '';

    // Sort events based on start time
    events.sort((a, b) => {
        const startA = new Date(a.start_time || a.conference?.start_time).getTime();
        const startB = new Date(b.start_time || b.conference?.start_time).getTime();
        return startA - startB;
    });

    // Render events into the timetable
    events.forEach(item => {
        const row = document.createElement('tr');

        const eventName = getEventName(item);
        const time = getTime(item);
        const description = getEventDescription(item);
        const detailsButton = `<button class="btn btn-link" onclick="viewDetails(${item.conference_id || item.conference?.id || item.id})">Details</button>`;

        row.innerHTML = `
            <td>${eventName}</td>
            <td>${time}</td>
            <td>${description}</td>
            <td>${detailsButton}</td>
        `;

        dataContainer.appendChild(row);
    });

    if (events.length === 0) {
        dataContainer.innerHTML = '<tr><td colspan="4" class="text-center">No events found.</td></tr>';
    }
}

// Helper function to get the ordinal suffix for a date
function getOrdinalSuffix(day) {
    if (day > 3 && day < 21) return 'th';
    switch (day % 10) {
        case 1: return 'st';
        case 2: return 'nd';
        case 3: return 'rd';
        default: return 'th';
    }
}

function getEventName(item) {
    let statusText = '';
    let statusClass = '';
    console.log(item.status);

    // Determine the event name based on item type
    switch (item.type) {
        case 'conference':
            return `<b>Conference</b>: ${item.name || 'Unnamed Conference'}`;
        case 'presentation':
            statusText = item.status ? item.status.charAt(0).toUpperCase() + item.status.slice(1) : '';
            statusClass = item.status === 'confirmed' ? 'status-confirmed' : 'status-unconfirmed';
            return `
                <div>
                    <b>Presentation</b>: ${item.name || 'Unnamed Presentation'}
                    ${statusText ? `<br><span class="${statusClass}">${statusText}</span>` : ''}
                </div>
            `;
        case 'reservation':
            statusText = item.status ? item.status.charAt(0).toUpperCase() + item.status.slice(1) : '';
            statusClass = {
                approved: 'status-approved',
                canceled: 'status-canceled',
                processing: 'status-processing'
            }[item.status] || '';
            return `
                <div>
                    <b>Reservation</b> for ${item.conference?.name || item.conference_name || 'Unnamed Conference'}
                    ${statusText ? `<br><span class="${statusClass}">${statusText}</span>` : ''}
                </div>
            `;
        default:
            return 'Unknown Event';
    }
}

function getTime(item) {
    const start = item.start_time || item.conference?.start_time;
    const end = item.end_time || item.conference?.end_time;

    if (item.type === 'presentation' && item.status === 'unconfirmed') {

        // If presentation is unconfirmed, use the conference time and indicate unconfirmed status
        const conferenceStart = new Date(item.start_time);
        const conferenceEnd = new Date(item.end_time);
        return `<b>Time Unconfirmed</b> (Between ${conferenceStart.toLocaleDateString('en-US', { month: 'long', day: 'numeric', year: 'numeric' })} ${conferenceStart.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })} - ${conferenceEnd.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })})`;
    }

    if (!start || !end) return 'N/A';

    const startDate = new Date(start);
    const endDate = new Date(end);

    const day = startDate.getDate();
    const ordinal = getOrdinalSuffix(day);

    return `${day}${ordinal} of ${startDate.toLocaleDateString('en-US', { month: 'long', year: 'numeric' })}<br>
            ${startDate.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })} - ${endDate.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })}`;
}

function getEventDescription(item) {
    return item.description || item.conference?.description || 'No description available.';
}

// Handle "Details" button click
window.viewDetails = function (conferenceId) {
    window.location.href = `/pages/conference.html?conference_id=${conferenceId}`;
};

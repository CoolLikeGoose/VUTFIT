import { baseUrl } from '../../config/config.js';

export function loadReservationsSection() {
    const reservationsSection = document.getElementById('reservations');
    reservationsSection.innerHTML = `
        <h2>Reservations</h2>
        <div id="reservations-container"></div>
    `;

    const conferenceId = getConferenceId();
    if (conferenceId) {
        loadReservations(conferenceId);
    }
}

function loadReservations(conferenceId) {
    fetch(`${baseUrl}/api/conferences/${conferenceId}/reservations`)
        .then(response => response.json())
        .then(reservations => {
            //show "processing" reservations first
            reservations.sort((a, b) => b.status.localeCompare(a.status));

            const container = document.getElementById('reservations-container');
            container.innerHTML = reservations.map(reservation => {
                //not showing canceled reservations
                if (reservation.status === "canceled") {
                    return '';
                }

                const actionButtons = reservation.status === "approved"
                    ? `<button class="btn btn-danger btn-sm" onclick="cancelReservation(${reservation.id})">Remove</button>`
                    : `
                        <button class="btn btn-success btn-sm mr-2" onclick="confirmReservation(${reservation.id})">Confirm</button>
                        <button class="btn btn-danger btn-sm" onclick="cancelReservation(${reservation.id})">Remove</button>
                    `;

                return `
                <div class="d-flex justify-content-between align-items-center border p-2 mb-2">
                    <div class="row">
                        <div class="field-column">
                            <p class><strong>Name:</strong> ${reservation.user.name || 'N/A'}</p>
                            <p><strong>Email:</strong> ${reservation.user.email || 'N/A'}</p>
                        </div>
                        <div class="field-column">
                            <p><strong>Visitors:</strong> ${reservation.visitors_count}</p>
                            <p><strong>Status:</strong> ${reservation.status}</p>
                        </div>
                    </div>
                    <div>
                        ${actionButtons}
                    </div>
                </div>
            `}).join('');
        })
        .catch(error => console.error('Error loading reservations:', error));
}

window.cancelReservation = function (reservationId) {
    fetch(`${baseUrl}/api/reservations/${reservationId}`, {
            method: 'PUT',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ status: 'canceled' })
        })
            .then(() => {
                const conferenceId = getConferenceId();
                loadReservations(conferenceId);
            })
            .catch(error => console.error('Error cancelling reservation:', error));
}

window.confirmReservation = function (reservationId) {
    fetch(`${baseUrl}/api/reservations/${reservationId}`, {
        method: 'PUT',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ status: 'approved' })
    })
        .then(() => {
            const conferenceId = getConferenceId();
            loadReservations(conferenceId);
        })
        .catch(error => console.error('Error confirming reservation:', error));
}

function getConferenceId() {
    const urlParams = new URLSearchParams(window.location.search);
    return urlParams.get('conference_id');
}

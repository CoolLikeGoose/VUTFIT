import { loadHeader } from '../loadHeader.js';
import { initNavBar } from './navbar.js';
import { loadMainInfoSection } from './mainInfo.js';
import { loadScheduleSection } from './schedule.js';
import { loadAuditoriumsSection } from './auditoriums.js';
import { loadReservationsSection } from './reservations.js';

function getConferenceId() {
    const urlParams = new URLSearchParams(window.location.search);
    return urlParams.get('conference_id');
}

document.addEventListener('DOMContentLoaded', () => {
    loadHeader();
    
    const confId = getConferenceId();

    initNavBar(confId);
    loadMainInfoSection();
    
    if (confId) {
        loadScheduleSection();
        loadAuditoriumsSection();
        loadReservationsSection();
    }
});

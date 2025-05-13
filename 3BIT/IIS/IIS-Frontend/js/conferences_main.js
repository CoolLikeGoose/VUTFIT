import { baseUrl } from "../config/config.js";

let currentPage = 0;
let selectedTags = [];
const MAX_TAGS = 3;

document.addEventListener("DOMContentLoaded", function () {
    loadConferences();
    loadPopularTags();

    // Event listeners for search and filters
    document.getElementById("search-input").addEventListener("input", () => {
        currentPage = 0;
        loadConferences();
    });

    document.getElementById("filter-time").addEventListener("change", () => {
        currentPage = 0;
        loadConferences();
    });

    document.getElementById("next-page-btn").addEventListener("click", () => {
        currentPage++;
        loadConferences();
    });

    document.getElementById("prev-page-btn").addEventListener("click", () => {
        if (currentPage > 0) {
            currentPage--;
            loadConferences();
        }
    });

    document.getElementById("tags-dropdown-menu").addEventListener("click", (e) => {
        if (e.target.classList.contains("tag-checkbox")) {
            const tag = e.target.dataset.tag;

            if (e.target.checked) {
                selectedTags.push(tag);
            } else {
                selectedTags = selectedTags.filter(t => t !== tag);
            }

            // Update the state of the checkboxes
            updateTagCheckboxStates();

            // Update dropdown text with selected tags
            updateTagsDropdownText();

            // Reload conferences based on new filters
            loadConferences();
        }
    });

    document.getElementById("clear-tags-btn").addEventListener("click", () => {
        selectedTags = [];
        document.querySelectorAll(".tag-checkbox").forEach(checkbox => {
            checkbox.checked = false;
        });

        // Update the state of the checkboxes
        updateTagCheckboxStates();

        // Update dropdown text with selected tags
        updateTagsDropdownText();

        // Reload conferences
        loadConferences();
    });
});

function loadConferences() {
    const searchQuery = document.getElementById("search-input").value;
    const filterTime = document.getElementById("filter-time").value;

    const conferenceList = document.getElementById("conference-list");
    const currentPageDisplay = document.getElementById("current-page");
    conferenceList.innerHTML = '<div class="col-12 text-center">Loading...</div>';

    // Prepare tags as query string
    const tagsQuery = selectedTags.length > 0 ? `&tags=${selectedTags.join(',')}` : '';

    const apiUrl = `${baseUrl}/api/conferences?search=${searchQuery}&filter_time=${filterTime}&start=${currentPage * 3}&limit=3${tagsQuery}`;

    fetch(apiUrl)
        .then(response => response.json())
        .then(data => {
            conferenceList.innerHTML = '';

            data.forEach(conference => {
                const conferenceCard = document.createElement("div");
                conferenceCard.className = "col-md-4 conference-card";

                // Generate tags HTML as Bootstrap badges
                let tagsHTML = "";
                if (Array.isArray(conference.tags)) {
                    conference.tags.forEach(tag => {
                        tagsHTML += `<span class="badge badge-primary mr-1">${tag}</span>`;
                    });
                }

                // Format start and end time to a single line with weekday and full date
                const startDate = new Date(conference.start_time);
                const endDate = new Date(conference.end_time);
                const formattedTime = `${startDate.toLocaleString('en-US', { weekday: 'long', month: 'long', day: 'numeric', year: 'numeric' })}, ${startDate.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })} - ${endDate.toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' })}`;

                // Determine if the conference is sold out
                let soldOutHTML = '';
                if (conference.current_occupancy >= conference.capacity) {
                    soldOutHTML = `<p class="text-danger font-weight-bold">Sold Out</p>`;
                }

                conferenceCard.innerHTML = `
                    <div class="card h-100">
                        <img src="${conference.picture}" class="card-img-top-main" alt="Conference Image">
                        <div class="card-body">
                            <h5 class="card-title">${conference.name}</h5>
                            <p class="card-text">${conference.description}</p>
                            ${soldOutHTML}
                            <p><strong>Cost:</strong> $${conference.cost}</p>
                            <p><strong>Capacity:</strong> ${conference.capacity} attendees</p>
                            <p><strong>Current Occupancy:</strong> ${conference.current_occupancy}</p>
                            <p><strong>Tags:</strong> ${tagsHTML}</p>
                            <p><strong>Time:</strong> ${formattedTime}</p>
                        </div>
                        <div class="card-footer">
                            <a href="/pages/conference.html?conference_id=${conference.id}" class="btn btn-primary">View Details</a>
                        </div>
                    </div>
                `;
                conferenceList.appendChild(conferenceCard);
            });

            if (data.length === 0) {
                conferenceList.innerHTML = '<div class="col-12 text-center">No conferences found.</div>';
            }

            // Update pagination display
            currentPageDisplay.textContent = `Page: ${currentPage + 1}`;

            // Disable/Enable pagination buttons
            document.getElementById("prev-page-btn").disabled = currentPage === 0;
            document.getElementById("next-page-btn").disabled = data.length < 3; // Disable if less than 3 items
        })
        .catch(error => {
            console.error("Error fetching conferences:", error);
            conferenceList.innerHTML = '<div class="col-12 text-center text-danger">Error loading conferences. Please try again later.</div>';
        });
}

function loadPopularTags() {
    const apiUrl = `${baseUrl}/api/conferences/tags/popular?filter_time=all`;

    fetch(apiUrl)
        .then(response => response.json())
        .then(tags => {
            const tagsDropdownMenu = document.getElementById("tags-dropdown-menu");

            tags.forEach(tag => {
                const tagItem = document.createElement("div");
                tagItem.className = "dropdown-item";
                tagItem.innerHTML = `
                    <input type="checkbox" class="tag-checkbox mr-2" data-tag="${tag}"> ${tag}
                `;
                tagsDropdownMenu.appendChild(tagItem);
            });
        })
        .catch(error => console.error("Error fetching popular tags:", error));
}

// Function to update dropdown button text based on selected tags
function updateTagsDropdownText() {
    const tagsDropdown = document.getElementById("tags-dropdown");
    if (selectedTags.length === 0) {
        tagsDropdown.textContent = "Select Tags";
    } else {
        tagsDropdown.textContent = selectedTags.join(', ');
    }
}

// Function to update the state of the checkboxes
function updateTagCheckboxStates() {
    const allCheckboxes = document.querySelectorAll(".tag-checkbox");
    allCheckboxes.forEach(checkbox => {
        if (selectedTags.length >= MAX_TAGS && !checkbox.checked) {
            // Disable checkboxes if the maximum number of tags has been reached
            checkbox.disabled = true;
        } else {
            // Enable checkboxes if under the maximum number of tags
            checkbox.disabled = false;
        }
    });
}

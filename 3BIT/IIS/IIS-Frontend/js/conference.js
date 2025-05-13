import { baseUrl } from "../config/config.js";

const user = JSON.parse(localStorage.getItem('user'));

const validateEmail = (email) => {
  return String(email)
    .toLowerCase()
    .match(
      /^(([^<>()[\]\\.,;:\s@"]+(\.[^<>()[\]\\.,;:\s@"]+)*)|.(".+"))@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\])|(([a-zA-Z\-0-9]+\.)+[a-zA-Z]{2,}))$/
    );
};

document.addEventListener("DOMContentLoaded", function () {
    const urlParams = new URLSearchParams(window.location.search);
    const conferenceId = urlParams.get('conference_id');

    if (!conferenceId) {
        alert("No conference ID provided.");
        return;
    }

    // Fetch conference details
    fetch(`${baseUrl}/api/conferences/${conferenceId}`)
        .then(response => response.json())
        .then(conference => {
            displayConferenceDetails(conference);
        })
        .catch(error => console.error('Error fetching conference details:', error));

    // Hide apply btn from unregistered users
    if (!user) {
      const applyBtn = document.getElementById("apply-presentation-btn");
      applyBtn.style = "display: none;"
    }
    
    // Fetch presentations related to the conference
    fetchPresentations(conferenceId);
});

function displayConferenceDetails(conference) {
    const conferenceContainer = document.getElementById("conference-details");
    if (!conference) {
        conferenceContainer.innerHTML = `<p>Conference details not found.</p>`;
        return;
    }

    const tags = conference.tags.length > 0 ? conference.tags.join(', ') : "No tags available";

    const isOwner = user && user.id === conference.organizer_id;

    conferenceContainer.innerHTML = `
    <div class="card">
      <img src="${conference.picture || '../assets/default-conference.png'}" class="card-img-top" alt="${conference.name}">
      <div class="card-body">
        <div class="nice-container">
          <h2 class="card-title">${conference.name}</h2>
          <button class="btn ${isOwner ? 'btn-warning' : 'btn-primary'} mt-3" id="conference-action-btn">
            ${isOwner ? 'Edit Conference' : 'Reserve'}
          </button> 
        </div>
        <p class="card-text">${conference.description}</p>
        <p><strong>Cost:</strong> $${conference.cost}</p>
        <p><strong>Capacity:</strong> ${conference.capacity} attendees</p>
        <p><strong>Current Occupancy:</strong> ${conference.current_occupancy}</p>
        <p><strong>Tags:</strong> ${tags}</p>
        <p><strong>Start Time:</strong> ${new Date(conference.start_time).toLocaleString()}</p>
        <p><strong>End Time:</strong> ${new Date(conference.end_time).toLocaleString()}</p>
      </div>
    </div>
    `;

    configureReservationBtn(conference, isOwner);
}

function configureReservationBtn(conference, isOwner) {
  const actionButton = document.getElementById('conference-action-btn');
    if (isOwner) {
        actionButton.addEventListener('click', () => {
            window.location.href = `/pages/edit_conference.html?conference_id=${conference.id}`;
        });
        return;
    }

    // if conference already full we need to disable reservation
    if (conference.capacity - conference.current_occupancy <= 0) {
      actionButton.disabled = true;
    }

    //if user is not logged in
    if (!user) {
      console.log(user)
      actionButton.addEventListener('click', () => openReservationPopup(conference, user));
      return;
    }

    //check if user already has reservation or presentation
    fetch(`${baseUrl}/api/conferences/${conference.id}/${user.id}`)
        .then(response => response.json())
        .then(info => {
            const reservation = info['reservation'].find(res => res.status !== "canceled");
            if (reservation) {
              actionButton.textContent = "Edit reservation";
              actionButton.disabled = false;
              actionButton.addEventListener('click', () => openReservationPopup(conference, user, reservation['id'], reservation['visitors_count']));
              return;
            }

            actionButton.addEventListener('click', () => openReservationPopup(conference, user));
        })
        .catch(error => console.error('Error fetching user info:', error));
}

function fetchPresentations(conferenceId) {
  fetch(`${baseUrl}/api/conferences/${conferenceId}/presentations`)
        .then(response => response.json())
        .then(presentations => {
            displayPresentations(presentations);
        })
        .catch(error => console.error('Error fetching presentations:', error));
}

function displayPresentations(presentations) {
    const presentationsList = document.getElementById("presentations-list");
    if (presentations.length === 0) {
        presentationsList.innerHTML = `<p>No presentations found for this conference.</p>`;
        return;
    }

    // Fetch the speaker information for each presentation
    Promise.all(presentations.map(presentation => {
        return fetch(`${baseUrl}/api/users/${presentation.speaker_id}`)
            .then(response => response.json())
            .then(user => ({
                ...presentation,
                speakerName: user ? user.name : 'Unknown Speaker'
            }))
            .catch(error => {
                console.error('Error fetching speaker:', error);
                return { ...presentation, speakerName: 'Unknown Speaker' };
            });
    }))
        .then(presentationsWithSpeakers => {
            //we need to sort presentations by status to show unconfirmed user presentations later
            presentationsWithSpeakers.sort((a, b) => b.status.localeCompare(a.status));

            presentationsList.innerHTML = presentationsWithSpeakers.map(presentation => {
            const isSpeaker = user ? (user.id == presentation.speaker_id) : false;
            
            if (presentation.status === "unconfirmed" && !isSpeaker) {
              //hide unconfirmed reservations from usual users
              return '';
            }
            
            return `
            <div class="card mb-3">
              ${presentation.status !== "unconfirmed" ? '' : "<h5>Your unconfirmed presentation</h5>"}
              <div class="row no-gutters">
                <div class="col-md-4">
                  <img src="${presentation.picture || '../assets/default-presentation.png'}" class="card-img" alt="${presentation.name}">
                </div>
                <div class="col-md-8">
                  <div class="card-body">
                    <h5 class="card-title">${presentation.name}</h5>
                    <p class="card-text">${presentation.description}</p>
                    <p class="card-text"><small class="text-muted">Presented by: ${presentation.speakerName}</small></p>
                    ${presentation.status !== "unconfirmed" ? `
                      <p class="card-text"><strong>Start Time:</strong> ${new Date(presentation.start_time).toLocaleString()}</p>
                      <p class="card-text"><strong>End Time:</strong> ${new Date(presentation.end_time).toLocaleString()}</p>
                      ${isSpeaker ? `
                      <button class="btn btn-primary" id="conference-edit-btn" onclick="editPresentation(${presentation.id})">Edit presentation</button>
                      ` : ''}
                    ` : `
                      <button class="btn btn-primary" id="conference-edit-btn" onclick="editPresentation(${presentation.id})">Edit presentation</button>
                    `}
                  </div>
                </div>
              </div>
            </div>
        `}).join('');
        })
        .catch(error => console.error('Error fetching presentations with speakers:', error));
}

window.editPresentation = function (presentationId) {
  fetch(`${baseUrl}/api/presentations/${presentationId}`)
        .then(response => response.json())
        .then(presentation => {
          openApplyPopup(presentationId.conference_id, presentation);
        })
        .catch(error => console.error('Error loading presentations:', error));
}
 
//--------------------------------
//------------Reserve-------------
//--------------------------------

function openReservationPopup(conference, user, res_id=-1, vis_cnt=-1) {
  const maxConferenceVisitors = conference.max_reservations_per_user;
  const availableCapacity = conference.capacity - conference.current_occupancy + (vis_cnt === -1 ? 0 : vis_cnt);

  const maxVisitors = Math.min(maxConferenceVisitors, availableCapacity);

  const modal = document.createElement('div');
  modal.classList.add('modal', 'fade');
  modal.id = 'reservationModal';
  modal.innerHTML = `
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">Reserve</h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body">
        <form id="reservation-form">
          <div class="form-group">
            <label for="reservation-name">Name</label>
            <input type="text" class="form-control" id="reservation-name" required value="${user ? user.name : ''}" ${user ? 'disabled' : ''}>
          </div>
          <div class="form-group">
            <label for="reservation-email">Email</label>
            <input type="email" class="form-control" id="reservation-email" required value="${user ? user.email : ''}" ${user ? 'disabled' : ''}>
          </div>
          <div class="form-group">
            <label for="visitors-count">Number of Visitors</label>
            <input type="number" class="form-control" id="visitors-count" min="1" max=${maxVisitors} required value="${vis_cnt != -1 ? vis_cnt : 1}">
            <small id="visitor-limit-msg" class="form-text text-muted">Visitors limit: ${maxVisitors}</small>
          </div>
        </form>
      </div>
      <div class="modal-footer nice-container">
        ${res_id != -1 ? `
          <button type="button" class="btn btn-danger" id="cancel-reservation-btn">Cancel reservation</button>
        `: ''}
        <button type="button" class="btn btn-secondary" data-dismiss="modal">Back</button>
        <button type="button" class="btn btn-primary" id="submit-reservation-btn">${res_id != -1 ? 'Save' : 'Reserve'}</button>
      </div>
    </div>
  </div>
`;

  document.body.appendChild(modal);

  $(modal).modal('show');

  //restrict user input of visitors by maxVisitors
  document.getElementById('visitors-count').addEventListener('input', () => {
    const input = event.target;
    const max = parseInt(input.max, 10);
    const value = parseInt(input.value, 10);

    if (value > max) {
        input.value = max;
    } else if (value < 1) {
        input.value = 1; 
    }
  });

  const cancelReservationBtn = document.getElementById('cancel-reservation-btn');
  if (cancelReservationBtn) {
    cancelReservationBtn.addEventListener('click', () => {
      cancelReservation(res_id);
  
      $(modal).modal('hide');
      modal.remove();
    });
  }

  document.getElementById('submit-reservation-btn').addEventListener('click', () => {
      const name = document.getElementById('reservation-name').value;
      const email = document.getElementById('reservation-email').value;
      const visitorsCount = parseInt(document.getElementById('visitors-count').value, 10);

      if (!validateEmail(email)) {
        alert('Please provide your real email');
        return;
      }

      if (!name || !email || !visitorsCount) {
          alert('Please fill in all fields.');
          return;
      }

      if (!user) {
          fetch(`${baseUrl}/api/users`, {
              method: 'POST',
              headers: { 'Content-Type': 'application/json' },
              body: JSON.stringify({ name, email, role: "user", password: " " })
          })
              .then(response => response.json())
              .then(newUser => {
                  makeReservation(conference.id, newUser.id, visitorsCount);
              })
              .catch(error => console.error('Error creating user:', error));
      } else {
          if (res_id != -1) {
            editReservation(res_id, visitorsCount);
          } else {
            makeReservation(conference.id, user.id, visitorsCount);
          }
      }

      $(modal).modal('hide');
      modal.remove();
  });
}

function cancelReservation(res_id) {
  fetch(`${baseUrl}/api/reservations/${res_id}`, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
        status: "canceled"
    })
})
    .then(response => response.json())
    .then(reservation => {
        alert('Reservation canceled successfully!');
        window.location.reload();
    })
    .catch(error => console.error('Error canceling reservation:', error));
}

function editReservation(res_id, visitorsCount) {
  fetch(`${baseUrl}/api/reservations/${res_id}`, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
        visitors_count: visitorsCount,
        status: "processing"
    })
})
    .then(response => response.json())
    .then(reservation => {
        alert('Reservation edited successfully!');
        window.location.reload();
    })
    .catch(error => console.error('Error editing reservation:', error));
}

function makeReservation(conferenceId, userId, visitorsCount) {
  fetch(`${baseUrl}/api/reservations`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({
          conference_id: conferenceId,
          user_id: userId,
          visitors_count: visitorsCount,
          status: 'processing'
      })
  })
      .then(response => {
        return response.json()
      })
      .then(reservation => {
          if (reservation['error']) {
            alert(reservation['error'])
          } else {
            alert('Reservation created successfully!');
          }
          window.location.reload();
      })
      .catch(error => console.error('Error making reservation:', error));
}


//--------------------------------
//-------------Apply--------------
//--------------------------------

document.addEventListener("DOMContentLoaded", function () {
  const urlParams = new URLSearchParams(window.location.search);
  const conferenceId = urlParams.get('conference_id');

  if (!conferenceId) {
      alert("No conference ID provided.");
      return;
  }

  document.getElementById('apply-presentation-btn').addEventListener('click', () => openApplyPopup(conferenceId));
});

function openApplyPopup(conferenceId, presentation=null) {
  if (!user) {
      alert('You need to be logged in to apply for a presentation.');
      return;
  }

  const modal = document.createElement('div');
  modal.classList.add('modal', 'fade');
  modal.id = 'applyPresentationModal';
  modal.innerHTML = `
  <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">${presentation ? 'Edit' : 'Apply for a'} Presentation</h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body">
        <form id="presentation-form">
          <div class="form-group">
            <label for="presentation-name">Presentation Name</label>
            <input type="text" class="form-control" id="presentation-name" required value="${presentation ? presentation.name : ''}">
          </div>
          <div class="form-group">
            <label for="presentation-description">Description</label>
            <textarea class="form-control" id="presentation-description" rows="3" required>${presentation ? presentation.description : ''}</textarea>
          </div>
          <div class="form-group">
            <label for="presentation-text">Presentation Text</label>
            <textarea class="form-control" id="presentation-text" rows="4" required>${presentation ? presentation.presentation_text : ''}</textarea>
          </div>
          <div class="form-group">
            <label for="presentation-picture">Picture (Optional)</label>
            <input type="url" class="form-control" id="presentation-picture" placeholder="URL to picture" value="${presentation ? presentation.picture : ''}">
          </div>
        </form>
      </div>
      <div class="modal-footer nice-container">
        ${presentation ? `
          <button type="button" class="btn btn-danger" id="delete-apply-btn">Delete presentation</button>
        `: ''}
        <button type="button" class="btn btn-secondary" data-dismiss="modal">Back</button>
        <button type="button" class="btn btn-primary" id="submit-presentation-btn">${presentation ? 'Save' : 'Submit'}</button>
      </div>
    </div>
  </div>
`;

  document.body.appendChild(modal);

  $(modal).modal('show');

  const cancelPresentationBtn = document.getElementById('delete-apply-btn');
  if (cancelPresentationBtn) {
    cancelPresentationBtn.addEventListener('click', () => {
      deletePresentation(presentation.id);
    });
  }

  document.getElementById('submit-presentation-btn').addEventListener('click', () => {
      const name = document.getElementById('presentation-name').value;
      const description = document.getElementById('presentation-description').value;
      const presentationText = document.getElementById('presentation-text').value;
      const picture = document.getElementById('presentation-picture').value;

      if (!name || !description || !presentationText) {
          alert('Please fill in all required fields.');
          return;
      }

      const presentationData = {
        speaker_id: user.id,
        conference_id: conferenceId,
        name,
        description,
        picture,
        presentation_text: presentationText,
        status: 'unconfirmed'
      };

      if (presentation) {
        presentationData['id'] = presentation.id;
        editPresentation(presentationData);
      } else {
        submitPresentation(presentationData);
      }

      $(modal).modal('hide');
      modal.remove();
  });
}

function deletePresentation(presentationId) {
  fetch(`${baseUrl}/api/presentations/${presentationId}`, {
    method: 'DELETE'
})
    .then(response => response.json())
    .then(presentation => {
        alert('Presentation deleted successfully!');
        window.location.reload();
    })
    .catch(error => console.error('Presentation deleted reservation:', error));
}

function editPresentation(presentationData) {
  fetch(`${baseUrl}/api/presentations/${presentationData['id']}`, {
    method: 'PUT',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(presentationData)
})
    .then(response => response.json())
    .then(presentation => {
        alert('Presentation edited successfully!');
        window.location.reload();
    })
    .catch(error => console.error('Error editing presentation:', error));
}

function submitPresentation(presentationData) {
  fetch(`${baseUrl}/api/presentations`, {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify(presentationData)
  })
      .then(response => {
          if (!response.ok) {
              throw new Error('Failed to submit presentation.');
          }
          return response.json();
      })
      .then(data => {
          alert('Presentation submitted successfully!');
          fetchPresentations(presentationData['conference_id']);
      })
      .catch(error => console.error('Error submitting presentation:', error));
}

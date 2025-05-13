import { baseUrl } from "../config/config.js";

let currentPageUsers = 0;  // Pagination tracking for users
let currentPageRooms = 0;  // Pagination tracking for rooms
const pageSizeUsers = 10;  // Number of users per page
const pageSizeRooms = 10;  // Number of rooms per page

document.addEventListener("DOMContentLoaded", function () {
    checkUserRole();
    loadUsers();
    loadRooms();

    // Event listeners for pagination
    document.getElementById("next-page-users-btn").addEventListener("click", () => {
        currentPageUsers++;
        loadUsers();
    });

    document.getElementById("prev-page-users-btn").addEventListener("click", () => {
        if (currentPageUsers > 0) {
            currentPageUsers--;
            loadUsers();
        }
    });

    document.getElementById("next-page-rooms-btn").addEventListener("click", () => {
        currentPageRooms++;
        loadRooms();
    });

    document.getElementById("prev-page-rooms-btn").addEventListener("click", () => {
        if (currentPageRooms > 0) {
            currentPageRooms--;
            loadRooms();
        }
    });

    // Event listeners for search and filter
    document.getElementById("user-search").addEventListener("input", () => {
        currentPageUsers = 0;
        loadUsers();
    });
    document.getElementById("user-filter").addEventListener("change", () => {
        currentPageUsers = 0;
        loadUsers();
    });
    document.getElementById("room-search").addEventListener("input", () => {
        currentPageRooms = 0;
        loadRooms();
    });
    document.getElementById("room-filter").addEventListener("input", () => {
        currentPageRooms = 0;
        loadRooms();
    });

    // Event listeners for add buttons
    document.getElementById('add-user-btn').addEventListener('click', () => openUserModal());
    document.getElementById('add-room-btn').addEventListener('click', () => openRoomModal());

    // Event listener for form submissions
    document.getElementById('user-form').addEventListener('submit', handleUserFormSubmit);
    document.getElementById('room-form').addEventListener('submit', handleRoomFormSubmit);
});

function checkUserRole() {
    const user = JSON.parse(localStorage.getItem('user')); // Retrieve user from local storage
    if (!user || user.role !== 'admin') {
        alert('You are not authorized to access this page.');
        window.location.href = '/pages/index.html'; // Redirect to the main page
    }
}

function loadUsers() {
    const searchInput = document.getElementById("user-search").value.trim();
    const filterInput = document.getElementById("user-filter").value;

    const start = currentPageUsers * pageSizeUsers;

    const params = new URLSearchParams({
        start,
        limit: pageSizeUsers + 1, // Fetch one extra record to determine if there's a next page
    });

    if (searchInput) {
        params.append("search", searchInput);
    }
    if (filterInput !== "all") {
        params.append("role", filterInput);
    }

    const apiUrl = `${baseUrl}/api/users?${params.toString()}`;
    fetch(apiUrl)
        .then(response => response.json())
        .then(users => {
            const usersList = document.getElementById("users-list");
            usersList.innerHTML = '';

            // Adjust fetched users for pagination
            const usersToShow = users.slice(0, pageSizeUsers);

            usersList.innerHTML = `
                <table class="table table-bordered">
                    <thead>
                        <tr>
                            <th>ID</th>
                            <th>Name</th>
                            <th>Email</th>
                            <th>Role</th>
                            <th>Actions</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${usersToShow.map(user => `
                            <tr>
                                <td>${user.id}</td>
                                <td>${user.name}</td>
                                <td>${user.email}</td>
                                <td>${user.role}</td>
                                <td>
                                    <button class="btn btn-sm btn-primary" onclick="editUser(${user.id})">Edit</button>
                                    <button class="btn btn-sm btn-danger" onclick="deleteUser(${user.id})">Delete</button>
                                </td>
                            </tr>
                        `).join('')}
                    </tbody>
                </table>
            `;

            // Update pagination display
            document.getElementById("current-page-users").textContent = `Page: ${currentPageUsers + 1}`;

            // Disable/Enable pagination buttons
            document.getElementById("prev-page-users-btn").disabled = currentPageUsers === 0;
            document.getElementById("next-page-users-btn").disabled = users.length <= pageSizeUsers;
        })
        .catch(error => console.error('Error fetching users:', error));
}

function loadRooms() {
    const searchInput = document.getElementById("room-search").value.trim();
    const filterInput = parseInt(document.getElementById("room-filter").value, 10);

    const start = currentPageRooms * pageSizeRooms;

    const params = new URLSearchParams({
        start,
        limit: pageSizeRooms + 1, // Fetch one extra record to determine if there's a next page
    });

    if (searchInput) {
        params.append("search", searchInput);
    }
    if (filterInput > 0) {
        params.append("min_capacity", filterInput);
    }

    const apiUrl = `${baseUrl}/api/auditoriums?${params.toString()}`;
    fetch(apiUrl)
        .then(response => response.json())
        .then(rooms => {
            const roomsList = document.getElementById("rooms-list");
            roomsList.innerHTML = '';

            // Adjust fetched rooms for pagination
            const roomsToShow = rooms.slice(0, pageSizeRooms);

            roomsList.innerHTML = `
                <table class="table table-bordered">
                    <thead>
                        <tr>
                            <th>ID</th>
                            <th>Room Name</th>
                            <th>Capacity</th>
                            <th>Actions</th>
                        </tr>
                    </thead>
                    <tbody>
                        ${roomsToShow.map(room => `
                            <tr>
                                <td>${room.id}</td>
                                <td>${room.name}</td>
                                <td>${room.capacity}</td>
                                <td>
                                    <button class="btn btn-sm btn-primary" onclick="editRoom(${room.id})">Edit</button>
                                    <button class="btn btn-sm btn-danger" onclick="deleteRoom(${room.id})">Delete</button>
                                </td>
                            </tr>
                        `).join('')}
                    </tbody>
                </table>
            `;

            // Update pagination display
            document.getElementById("current-page-rooms").textContent = `Page: ${currentPageRooms + 1}`;

            // Disable/Enable pagination buttons
            document.getElementById("prev-page-rooms-btn").disabled = currentPageRooms === 0;
            document.getElementById("next-page-rooms-btn").disabled = rooms.length <= pageSizeRooms;
        })
        .catch(error => console.error('Error fetching rooms:', error));
}

// Modal opening functions
function openUserModal(user = null) {
    const modal = $('#userModal');
    document.getElementById('user-id').value = user ? user.id : '';
    document.getElementById('fuser-name').value = user ? user.name : '';
    document.getElementById('user-email').value = user ? user.email : '';
    document.getElementById('user-role').value = user ? user.role : 'user';
    modal.modal('show');
}

function openRoomModal(room = null) {
    const modal = $('#roomModal');
    document.getElementById('room-id').value = room ? room.id : '';
    document.getElementById('room-name').value = room ? room.name : '';
    document.getElementById('room-capacity').value = room ? room.capacity : '';
    modal.modal('show');
}

// User form submission handler
function handleUserFormSubmit(event) {
    event.preventDefault();
    const id = document.getElementById('user-id').value.trim();
    const name = document.getElementById('fuser-name').value.trim();
    const email = document.getElementById('user-email').value.trim();
    const role = document.getElementById('user-role').value.trim();

    const method = id ? 'PUT' : 'POST';
    const url = id ? `${baseUrl}/api/users/${id}` : `${baseUrl}/api/users`;

    fetch(url, {
        method: method,
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, email, role })
    })
        .then(response => {
            if (response.ok) {
                return response.json();
            } else if (response.status === 409) {
                throw new Error('Email already exists.');
            } else {
                return response.json().then(data => {
                    throw new Error(data.message || 'Error saving user.');
                });
            }
        })
        .then(() => {
            $('#userModal').modal('hide');
            loadUsers();
        })
        .catch(error => {
            alert(error.message);
            console.error('Error saving user:', error);
        });
}

// Room form submission handler
function handleRoomFormSubmit(event) {
    event.preventDefault();
    const id = document.getElementById('room-id').value.trim();
    const name = document.getElementById('room-name').value.trim();
    const capacity = parseInt(document.getElementById('room-capacity').value.trim(), 10);

    const method = id ? 'PUT' : 'POST';
    const url = id ? `${baseUrl}/api/auditoriums/${id}` : `${baseUrl}/api/auditoriums`;

    fetch(url, {
        method: method,
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ name, capacity })
    })
        .then(response => {
            if (response.ok) {
                return response.json();
            } else if (response.status === 409) {
                throw new Error('Room with this name already exists.');
            } else {
                return response.json().then(data => {
                    throw new Error(data.message || 'Error saving room.');
                });
            }
        })
        .then(() => {
            $('#roomModal').modal('hide');
            loadRooms();
        })
        .catch(error => {
            alert(error.message);
            console.error('Error saving room:', error);
        });
}

// User actions
window.editUser = function (userId) {
    fetch(`${baseUrl}/api/users/${userId}`)
        .then(response => response.json())
        .then(user => openUserModal(user))
        .catch(error => console.error('Error fetching user:', error));
};

window.deleteUser = function (userId) {
    if (confirm(`Are you sure you want to delete user ${userId}?`)) {
        fetch(`${baseUrl}/api/users/${userId}`, { method: 'DELETE' })
            .then(() => loadUsers())
            .catch(error => console.error('Error deleting user:', error));
    }
};

// Room actions
window.editRoom = function (roomId) {
    fetch(`${baseUrl}/api/auditoriums/${roomId}`)
        .then(response => response.json())
        .then(room => openRoomModal(room))
        .catch(error => console.error('Error fetching room:', error));
};

window.deleteRoom = function (roomId) {
    if (confirm(`Are you sure you want to delete room ${roomId}?`)) {
        fetch(`${baseUrl}/api/auditoriums/${roomId}`, { method: 'DELETE' })
            .then(() => loadRooms())
            .catch(error => console.error('Error deleting room:', error));
    }
};

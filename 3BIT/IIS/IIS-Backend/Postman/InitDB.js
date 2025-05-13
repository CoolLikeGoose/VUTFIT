console.log("Initialization Start");

const auditoriumsData = [
    { name: "D105", capacity: 300 },
    { name: "D206", capacity: 40 },
    { name: "D207", capacity: 100 },
    { name: "B101", capacity: 50 },
    { name: "A304", capacity: 150 },
];

const usersData = [
    { name: "Alice Johnson", email: "alice.johnson@example.com", role: "admin", picture: "https://example.com/pictures/alice.jpg" },
    { name: "Bob Smith", email: "bob.smith@example.com", role: "user", picture: "https://example.com/pictures/bob.jpg" },
    { name: "Charlie Brown", email: "charlie.brown@example.com", role: "user", picture: "https://example.com/pictures/charlie.jpg" },
    { name: "Dana White", email: "dana.white@example.com", role: "admin", picture: "https://example.com/pictures/dana.jpg" },
    { name: "Evan Green", email: "evan.green@example.com", role: "user", picture: "https://example.com/pictures/evan.jpg" },
];

const conferencesData = [
    { 
        orginizer_id: 1, 
        name: "Tech Innovations 2024", 
        description: "Annual conference on the latest in technology and innovation.", 
        capacity: 500, 
        cost: 150, 
        max_reservations_per_user: 3, 
        current_occupancy: 350, 
        picture: "https://example.com/pictures/tech_innovations.jpg", 
        tags: ["technology", "innovation", "2024"], 
        start_time: "2024-05-15T09:00:00Z", 
        end_time: "2024-05-15T17:00:00Z" 
    },
    // { 
    //     orginizer_id: 2, 
    //     name: "Health & Wellness Expo", 
    //     description: "Explore new trends in health and wellness.", 
    //     capacity: 300, 
    //     cost: 50, 
    //     max_reservations_per_user: 2, 
    //     current_occupancy: 200, 
    //     picture: "https://example.com/pictures/health_expo.jpg", 
    //     tags: ["health", "wellness", "expo"], 
    //     start_time: "2024-06-20T10:00:00Z", 
    //     end_time: "2024-06-20T18:00:00Z" 
    // },
    // { 
    //     orginizer_id: 3, 
    //     name: "Green Energy Summit", 
    //     description: "A gathering of industry leaders to discuss renewable energy.", 
    //     capacity: 400, 
    //     cost: 100, 
    //     max_reservations_per_user: 4, 
    //     current_occupancy: 150, 
    //     picture: "https://example.com/pictures/green_energy.jpg", 
    //     tags: ["energy", "green", "summit"], 
    //     start_time: "2024-07-10T08:00:00Z", 
    //     end_time: "2024-07-10T16:00:00Z" 
    // },
    // { 
    //     orginizer_id: 4, 
    //     name: "Digital Marketing World", 
    //     description: "Insights into the world of digital marketing.", 
    //     capacity: 250, 
    //     cost: 75, 
    //     max_reservations_per_user: 2, 
    //     current_occupancy: 125, 
    //     picture: "https://example.com/pictures/marketing_world.jpg", 
    //     tags: ["digital marketing", "marketing", "2024"], 
    //     start_time: "2024-08-05T09:30:00Z", 
    //     end_time: "2024-08-05T17:00:00Z" 
    // },
    // { 
    //     orginizer_id: 5, 
    //     name: "Artificial Intelligence Symposium", 
    //     description: "Deep dive into AI and machine learning trends.", 
    //     capacity: 600, 
    //     cost: 200, 
    //     max_reservations_per_user: 5, 
    //     current_occupancy: 450, 
    //     picture: "https://example.com/pictures/ai_symposium.jpg", 
    //     tags: ["AI", "machine learning", "symposium"], 
    //     start_time: "2024-09-12T09:00:00Z", 
    //     end_time: "2024-09-12T18:00:00Z" 
    // }
];

const presentationsData = [
    { 
        speaker_id: 2, 
        conference_id: 1, 
        name: "The Future of AI", 
        description: "Exploring advancements and ethical considerations in artificial intelligence.", 
        picture: "https://example.com/pictures/ai_future.jpg", 
        presentation_text: "In this talk, we will dive into the latest breakthroughs in AI...", 
        auditorium_id: 1, 
        status: "scheduled", 
        start_time: "2024-05-15T10:00:00Z", 
        end_time: "2024-05-15T11:30:00Z" 
    },
    // { 
    //     speaker_id: 2, 
    //     conference_id: 2, 
    //     name: "Wellness in the Workplace", 
    //     description: "Strategies to promote health and wellness among employees.", 
    //     picture: "https://example.com/pictures/wellness_workplace.jpg", 
    //     presentation_text: "This session covers effective wellness programs and initiatives...", 
    //     auditorium_id: 102, 
    //     status: "scheduled", 
    //     start_time: "2024-06-20T11:00:00Z", 
    //     end_time: "2024-06-20T12:30:00Z" 
    // },
    // { 
    //     speaker_id: 3, 
    //     conference_id: 3, 
    //     name: "Renewable Energy Challenges", 
    //     description: "Discussing barriers and solutions in renewable energy adoption.", 
    //     picture: "https://example.com/pictures/energy_challenges.jpg", 
    //     presentation_text: "This talk will examine the key obstacles in renewable energy...", 
    //     auditorium_id: 103, 
    //     status: "scheduled", 
    //     start_time: "2024-07-10T14:00:00Z", 
    //     end_time: "2024-07-10T15:30:00Z" 
    // },
    // { 
    //     speaker_id: 4, 
    //     conference_id: 4, 
    //     name: "Social Media Strategies", 
    //     description: "Effective techniques for digital marketing through social media.", 
    //     picture: "https://example.com/pictures/social_media.jpg", 
    //     presentation_text: "In today's session, we'll explore ways to leverage social media...", 
    //     auditorium_id: 104, 
    //     status: "scheduled", 
    //     start_time: "2024-08-05T13:00:00Z", 
    //     end_time: "2024-08-05T14:30:00Z" 
    // },
    // { 
    //     speaker_id: 5, 
    //     conference_id: 5, 
    //     name: "Machine Learning in Healthcare", 
    //     description: "How machine learning is transforming the healthcare industry.", 
    //     picture: "https://example.com/pictures/ml_healthcare.jpg", 
    //     presentation_text: "This talk focuses on machine learning applications in healthcare...", 
    //     auditorium_id: 105, 
    //     status: "scheduled", 
    //     start_time: "2024-09-12T09:30:00Z", 
    //     end_time: "2024-09-12T11:00:00Z" 
    // }
];

const reservationsData = [
    { 
        conference_id: 1, 
        user_id: 3, 
        visitors_count: 2, 
        status: "confirmed" 
    },
    // { 
    //     conference_id: 2, 
    //     user_id: 2, 
    //     visitors_count: 1, 
    //     status: "pending" 
    // },
    // { 
    //     conference_id: 3, 
    //     user_id: 3, 
    //     visitors_count: 4, 
    //     status: "cancelled" 
    // },
    // { 
    //     conference_id: 4, 
    //     user_id: 4, 
    //     visitors_count: 3, 
    //     status: "confirmed" 
    // },
    // { 
    //     conference_id: 5, 
    //     user_id: 5, 
    //     visitors_count: 1, 
    //     status: "pending" 
    // }
];


function createData(data, api_type) {
    pm.sendRequest({
        url: pm.collectionVariables.get("base_url") + "/api/" + api_type,
        method: "POST",
        header: { "Content-Type": "application/json" },
        body: {
            mode: "raw",
            raw: JSON.stringify(data)
        }
    }, (err, res) => {
        if (err) {
            console.log("Error while creating: ", err);
        } else {
            console.log("Created: for "+api_type, res.json());
        }
    });
}

function checkData(data, api_type) {

    pm.sendRequest(pm.collectionVariables.get("base_url") + "/api/" + api_type, function (err, res) {
        if (err) {
            console.log("Error while checking: " + api_type, err);
            return;
        }
        const countAfterDelete = res.json().length;
        pm.test("All " + api_type + " was created successfully", function () {
            pm.expect(countAfterDelete).to.equal(data.length);
        });
    });
}

auditoriumsData.forEach(data => createData(data, "auditoriums"));
checkData(auditoriumsData, "auditoriums");

usersData.forEach(data => createData(data, "users"));
checkData(usersData, "users");

conferencesData.forEach(data => createData(data, "conferences"));
checkData(conferencesData, "conferences");

presentationsData.forEach(data => createData(data, "presentations"));
checkData(presentationsData, "presentations");

reservationsData.forEach(data => createData(data, "reservations"));
checkData(reservationsData, "reservations");

console.log("Initialization End");

Appointment Booking System - Frontend
---

Directory Structure

Root Directory
- `README.md`: General project documentation in markdown format.
- `readme.txt`: Detailed project explanation, including authorship and file descriptions.
- `package.json` & `package-lock.json`: Node.js package configuration and dependency management files.
- `webpack.config.js`: Webpack configuration for bundling the frontend assets.
- `deployment/`: Contains bundled files for production deployment.
  - `bundle.js`: Compiled JavaScript bundle for the app.
  - `bundle.js.LICENSE.txt`: Licensing information for the bundle.
  - `index.html`: Entry point for the deployment build.

---

Source Code (`src`)

Main Files
- `App.js` - Vladyslav Yeroma (xyerom00): Entry file for the application. Initializes and renders the app, providing navigation links to various interfaces.
- `index.js` - Vladyslav Yeroma (xyerom00): Entry point for React, rendering the app to the DOM and setting up routes.

---

Components

Appointments (`components/Appointments`)
- `AppointmentForm.js` - Vladyslav Yeroma (xyerom00): Handles the editing and rescheduling of appointments, including real-time slot recalculation.
- `AppointmentOptionsOverlay.js` - Vladyslav Yeroma (xyerom00): Displays quick action buttons (e.g., cancel, reschedule) for appointments in an interactive popup.

Booking (`components/Booking`)
- `BookForm.js` Nikita Smirnov (xsmirn02): Form for unregistered users to book appointments.
- `CalendarDescription.js` Nikita Smirnov (xsmirn02): Displays calendar-specific details for users during the booking process.
- `TimeSlots.js` Nikita Smirnov (xsmirn02): Lists available time slots for a selected date.

Calendar (`components/Calendar`)
- `BookingCalendar.js` Nikita Smirnov (xsmirn02): Calendar interface for unregistered users to view and book available appointments.
- `CalendarHeader.js` - Vladyslav Yeroma (xyerom00): Navigation header for the calendar view, including week/day toggle and navigation controls.
- `CalendarView.js`- Vladyslav Yeroma (xyerom00): Main calendar view displaying appointments with color-coded provider data.
- `SidebarCalendar.js`- Vladyslav Yeroma (xyerom00): Sidebar with provider filters and date navigation controls.

Provider (`components/Provider`)
- `ProviderInfo.js` - Zverev Daniil (xzvere00): Displays summarized provider information.
- `ProviderInfoView.js` - Zverev Daniil (xzvere00): A detailed view of provider information.
- `ProviderProfile.js` - Zverev Daniil (xzvere00): Interface for creating or managing provider profiles.
- `ProviderProfileView.js` - Zverev Daniil (xzvere00): Detailed view for editing provider profiles.

Statistics (`components/Statistics`)
- `AppointmentChart.js`: Visualizes appointment statistics in chart form.
- `BarChart.js`, `LineChart.js`: Components for rendering bar and line charts for analytics.
- `Selectors.js`: Dropdowns and checkboxes for filtering data in the analytics dashboard.
- `OverallStats.js`: Displays key metrics (e.g., total bookings, cancellations).
- `StatsInfo.js`: Detailed statistics view for admins.

Calendars (`components/calendars`)
- `CalendarManagement.js` - Zverev Daniil (xzvere00): Interface for creating and editing calendar configurations.
- `CalendarManagementView.js` - Zverev Daniil (xzvere00): Detailed view for managing calendars.
- `CalendarSettings.js` - Zverev Daniil (xzvere00): Page for configuring calendar attributes.
- `CalendarSettingsView.js` - Zverev Daniil (xzvere00): Detailed settings view for calendars.

---

Controllers
Custom hooks for managing data fetching, state management, and backend communication.

- `useAppointmentsController.js` - Vladyslav Yeroma (xyerom00): Manages fetching, updating, and canceling appointments.
- `useProvidersController.js` - Vladyslav Yeroma (xyerom00): Fetches and manages provider data, including filtering logic.
- `useReschedule.js`- Vladyslav Yeroma (xyerom00): Retrieves available slots for rescheduling appointments and handles slot recalculations.
- `useCalendarController.js` - Vladyslav Yeroma (xyerom00): Centralized logic for managing calendar data and backend integration.
- `useCalendarSettingsController.js` - Zverev Daniil (xzvere00): Handles settings for individual calendars.
- `useCalendarManagementController.js` - Zverev Daniil (xzvere00): Manages the creation and editing of calendars.
- `useProviderInfoController.js` - Zverev Daniil (xzvere00): Fetches and manages detailed provider information.
- `useProviderProfileController.js` - Zverev Daniil (xzvere00): Manages provider profile creation and updates.
- `useBookingController.js` Nikita Smirnov (xsmirn02): Handles user bookings and available slots.

---

Pages
Defines application-level routes for different user interfaces.

- `BookingPage.js` Nikita Smirnov (xsmirn02): Interface for unregistered users to view calendars and book appointments.
- `CalendarPage.js` - Vladyslav Yeroma (xyerom00): Admin interface for managing appointments, rescheduling, and cancellations.
- `StatsPage.js`: Analytics dashboard for viewing business insights and metrics.
- `CalendarInfoPage.js` - Zverev Daniil (xzvere00): Displays all available calendars with detailed information.
- `CalendarManagementPage.js` - Zverev Daniil (xzvere00): Interface for creating and managing calendars.
- `ProviderInfoPage.js` - Zverev Daniil (xzvere00): Displays detailed information about providers.
- `ProviderProfilePage.js` - Zverev Daniil (xzvere00): Interface for editing or creating provider profiles.

---

Services
Abstraction layer for communicating with the backend APIs.

- `appointmentsService.js` - Vladyslav Yeroma (xyerom00) and Nikita Smirnov (xsmirn02): Handles CRUD operations for appointments, including updates and cancellations.
- `calendarService.js` - Vladyslav Yeroma (xyerom00) and Zverev Daniil (xzvere00): Fetches calendar data and updates calendar attributes.
- `clientsService.js`- Vladyslav Yeroma (xyerom00) and Nikita Smirnov (xsmirn02): Manages client information and data fetching.
- `freeSlotsService.js` - Vladyslav Yeroma (xyerom00): Retrieves available time slots for appointments dynamically.
- `providerService.js` - Vladyslav Yeroma (xyerom00) and Zverev Daniil (xzvere00): Handles API requests related to providers.
- `statsService.js`: Fetches statistical data for business analytics.
- `config.js` - Vladyslav Yeroma (xyerom00): Stores the base URL for backend communication.

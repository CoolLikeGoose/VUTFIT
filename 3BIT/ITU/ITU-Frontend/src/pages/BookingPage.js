/**
 * @file BookingPage.js
 * @description The main page component for displaying the booking interface. 
 *              Allows users to select a date, time, and provider for an appointment.
 * @author Nikita Smirnov (xsmirn02)
 */

import React, { useEffect, useState } from 'react';
import { CssBaseline, Container } from '@mui/material';
import { CalendarDescription } from '../components/Booking/CalendarDescription';
import { BookingCalendar } from '../components/Calendar/BookingCalendar';
import { TimeSlots } from '../components/Booking/TimeSlots';
import { useBookingController } from '../controllers/useBookingController';
import '../styles/booking_page.css';
import { BookForm } from '../components/Booking/BookForm';

import {calendarService} from '../services/calendarService'

const BookingPage = () => {
    // DEBUGGING: Set initial calendarId for testing
    // TODO: Refactor to avoid multiple calendar fetches on date change
    const [calendarId, setCalendarId] = useState(14);
    calendarService.getAllCalendars().then((value) => setCalendarId(value[0]['id']));
    // DEBUGGING

    const [selectedProvider, setSelectedProvider] = useState(null);
    const [appointmentProviderID, setAppointmentProviderID] = useState(null);
    const [selectedDate, setSelectedDate] = useState(new Date());
    const [formatedDate, setFormatedDate] = useState("Loading...");
    const [availableTimeSlots, setAvailableTimeSlots] = useState({});
    const [displayedTimeSlots, setDisplayedTimeSlots] = useState({});
    const [selectedTime, setSelectedTime] = useState(null);
    const [isConfirmed, setIsConfirmed] = useState(false);

    const handleDateChange = async (date) => {
        setSelectedTime(null);
        setSelectedDate(date);
        const options = { weekday: 'short', month: 'long', day: 'numeric' };
        setFormatedDate(new Intl.DateTimeFormat('en-US', options).format(date));

        if (availableTimeSlots[date.toJSON().slice(0, 10)]) {
            setDisplayedTimeSlots(Object.entries(availableTimeSlots[date.toJSON().slice(0, 10)]));
        } else {
            setDisplayedTimeSlots([]);
        }
    };
    
    
    //mostly calendar details
    const { calendarDetails, avaibleProviders, avaibleDates } = useBookingController({calendarId, selectedProvider, selectedDate});

    const [calendarName, setCalendarName] = useState("Loading..."); 
    const [calendarImg, setcalendarImg] = useState("https://cloud-atg.moph.go.th/quality/sites/default/files/default_images/default.png"); 
    const [calendarDescription, setCalendarDescription] = useState("Loading..."); 
    const [calendarDuration, setcalendarDuration] = useState(15); 
    const [calendarPlace, setcalendarPlace] = useState("Loading..."); 
    useEffect(() => {
        if (calendarDetails) {
            setCalendarName(calendarDetails['name']);
            setcalendarImg(calendarDetails['img']);
            setCalendarDescription(calendarDetails['description']);
            setcalendarDuration(calendarDetails['default_duration']);
            setcalendarPlace(calendarDetails['place']);
        }
    }, [calendarDetails]);

    // Processes available dates and formats them into time slots grouped by date
    useEffect(() => {
        if (avaibleDates.length == 0) return;
        const timeSlots = {};

        for (const providerId in avaibleDates) {
            avaibleDates[providerId].forEach((timeString) => {
                // const utcTime = timeString.split(/[-T:]/);
                // const localTime = new Date(Date.UTC(utcTime[0], utcTime[1]-1, utcTime[2], utcTime[3], utcTime[4]));

                const [date, time] = timeString.split('T');
                const formatedTime = time.substring(0,5);

                if (!timeSlots[date]) {
                    timeSlots[date] = {};
                }

                if (!timeSlots[date][formatedTime]) {
                    timeSlots[date][formatedTime] = [];
                }

                timeSlots[date][formatedTime].push(providerId);
            });
        }

        setAvailableTimeSlots(timeSlots);
    }, [avaibleDates]);

    // Automatically updates displayed time slots when availability changes
    useEffect(() => {
        if (!availableTimeSlots || Object.keys(availableTimeSlots).length == 0) return;
        if (selectedDate.toJSON().slice(0, 10) == new Date().toJSON().slice(0, 10)) {
            handleDateChange(new Date());
        }
    }, [availableTimeSlots]);

    return (
        <Container className='booking-page'
            sx={{
                display: 'flex',
                justifyContent: 'center',
                alignItems: 'center',
                height: '100vh',
                fontFamily: 'Poppins, sans-serif', 
            }}
        >
            <CssBaseline />
            {/* calendar details */}
            <div className='calendar-details'>
                <CalendarDescription
                    eventName={calendarName}
                    eventImg={calendarImg}
                    details={calendarDescription}
                    duration={calendarDuration}
                    place={calendarPlace}
                    avaibleProviders={avaibleProviders}
                    setSelectedProvider={setSelectedProvider}
                    appointmentProviderID={appointmentProviderID}
                />
            </div>
            <div className={`calendar ${isConfirmed ? 'hidden' : ''}`}>
                <BookingCalendar
                    selectedDate={selectedDate}
                    setSelectedDate={handleDateChange}    
                    availableTimeSlots={availableTimeSlots}
                />
            </div>
            <div className={`form-section ${isConfirmed ? 'expanded' : ''}`}>
                <div className={`form-input ${isConfirmed ? '' : 'hidden'}`}>
                    <BookForm
                        setIsConfirmed={setIsConfirmed}
                        setAppointmentProviderID={setAppointmentProviderID}
                        appointmentProviderID={appointmentProviderID}
                        calendarId={calendarId}
                        calendarDuration={calendarDuration}
                        selectedTime={selectedTime}
                        selectedDate={selectedDate}
                        formatedDate={formatedDate}
                    />
                </div>

                <div className={`time-selector ${isConfirmed ? 'hidden' : ''}`}>
                    <TimeSlots
                        formatedDate={formatedDate}
                        setSelectedTime={setSelectedTime}
                        timeSlots={displayedTimeSlots}
                        setIsConfirmed={setIsConfirmed}
                        setAppointmentProviderID={setAppointmentProviderID}
                    />
                </div>
            </div>
        </Container>
    );
};

export default BookingPage;
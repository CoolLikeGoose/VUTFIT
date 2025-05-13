/**
 * @file BookingPage.js
 * @description Mainly used to dynamically download information about available dates and times for later registration by the user.
 * @author Nikita Smirnov (xsmirn02)
 */

import { useEffect, useState } from 'react';
import { calendarService } from '../services/calendarService';
import { freeSlotsService } from '../services/freeSlotsService';

export const useBookingController = ( { calendarId, selectedProvider, selectedDate } ) => {
    const [calendarDetails, setCalendarDetails] = useState(null);
    const [avaibleProviders, setAvaibleProviders] = useState([]);
    const [avaibleDates, setAvaibleDates] = useState([]);

    useEffect(() => {
        if (calendarId == -1) return;
        const fetchCalendar = async (calendarId) => {
            try {
                const result = await calendarService.getCalendarById(calendarId);
                setCalendarDetails(result);
            } catch (error) {
                console.error('Failed to load calendar:', error);
            }
        };

        fetchCalendar(calendarId);
    }, [calendarId]);

    useEffect(() => {
        if (calendarId == -1) return;
        const fetchProviders = async (calendarId) => {
            try {
                const result = await calendarService.getProvidersForCalendar(calendarId);
                setAvaibleProviders(result);
            } catch (error) {
                console.error('Failed to load calendar:', error);
            }
        };

        fetchProviders(calendarId);
    }, [calendarId]);

    useEffect(() => {
        const loadAvailableDates = async (selectedProvider, selectedDate) => {
            if (avaibleProviders.length == 0) return;

            let providersIds = 0;
            if (selectedProvider) {
                providersIds = [selectedProvider];
            } else {
                providersIds = avaibleProviders.map((provider) => (provider['id']));
            }

            const year = selectedDate.getFullYear();
            const month = selectedDate.getMonth();
            const free_slots = freeSlotsService.getFreeSlots(
                (new Date(year, month, 1)).toISOString(),
                (new Date(year, month+1, 1)).toISOString(),
                providersIds,
                calendarDetails['default_duration']
            );

            free_slots.then(((value) => setAvaibleDates(value)));
        };

        loadAvailableDates(selectedProvider, selectedDate);
    }, [selectedProvider, avaibleProviders]);

    return { calendarDetails, avaibleProviders, avaibleDates };
};

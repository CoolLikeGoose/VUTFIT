/**
 * @file BookingCalendar.js
 * @description Provides a user-friendly interface for selecting the preferred appointment time.
 * @author Nikita Smirnov (xsmirn02)
 */

import React, { useState } from 'react';
import { DateCalendar } from '@mui/x-date-pickers/DateCalendar';
import { LocalizationProvider } from '@mui/x-date-pickers';
import { AdapterDateFns } from '@mui/x-date-pickers/AdapterDateFnsV3';
import { Box } from '@mui/material';

export const BookingCalendar = ({selectedDate, setSelectedDate, availableTimeSlots}) => {
    const handleChange = (newDate) => {
        setSelectedDate(newDate);
    };

    const isDayAvaible = (day) => {
        const year = day.getFullYear();
        const month = day.getMonth()+1;
        const date = day.getDate() / 10 < 1 ? '0'+day.getDate() : day.getDate();
        const dateString = year+'-'+month+'-'+date;
        return availableTimeSlots.hasOwnProperty(dateString);
    };

    return (
        <LocalizationProvider dateAdapter={AdapterDateFns}>
            <Box>
                {/* <div>UTC huitisi atd.</div> */}
                <DateCalendar
                    value={selectedDate}
                    onChange={handleChange}
                    slotProps={{
                        day: (day) => ({
                            style: {
                                backgroundColor: isDayAvaible(day.day) ? '#dce9fd' : undefined,
                            }
                        }),
                    }}
                />
            </Box>
        </LocalizationProvider>
    );
};
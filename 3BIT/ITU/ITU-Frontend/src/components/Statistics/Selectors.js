import {Box, Checkbox, FormControl, FormControlLabel, InputLabel, MenuItem, Select,} from "@mui/material";
import React, { useEffect, useState } from 'react';
import {providerService} from "../../services/providerService";
import {calendarService} from "../../services/calendarService";

const Selectors = () => {
    const [providers, setProviders] = useState([]);
    const [calendars, setCalendars] = useState([]);
    const [showProviderSelect, setShowProviderSelect] = useState(true);
    const [selectedTimeInterval, setSelectedTimeInterval] = useState('AllTime');
    const [selectedCalendar, setSelectedCalendar] = useState('');
    const [selectedProvider, setSelectedProvider] = useState('');
    const [error, setError] = useState(null);

    const handleChangeTime = (event) => {
        setSelectedTimeInterval(event.target.value);
    };
    const handleChangeProvider = (event) => {
        setSelectedProvider(event.target.value);
    };

    const handleChangeCalendar = (event) => {
        setSelectedCalendar(event.target.value);
    };

    const handleCheckboxChange = (event) => {
        setShowProviderSelect(event.target.checked);
    };


    useEffect(() => {
        const loadProviders = async () => {
            try {
                const allProviders = await providerService.getProviders();
                setProviders(allProviders);
            } catch (err) {
                setError(err.message || 'Failed to load providers');
                console.error('Error loading providers:', err);
            }
        };
        loadProviders();
    }, []);

    useEffect(() => {
        const loadCalendars = async () => {
            try {
                const allCalendars = await calendarService.getAllCalendars();
                setCalendars(allCalendars);
            } catch (err) {
                setError(err.message || 'Failed to load calendars');
                console.error('Error loading calendars:', err);
            }
        };
        loadCalendars();
    }, []);

    return (
    <Box p={3} sx={{ backgroundColor: 'rgba(255, 255, 255, 0.4)', boxShadow: 3 }} display="flex" flexDirection="row" gap={2}>
        <FormControl fullWidth sx={{ width: '50%' }}>
            <InputLabel id="IntervalsSelect" style={{ color: '#000000' }}>
                Select time interval
            </InputLabel>
            <Select
                labelId="IntervalsSelectLabel"
                id="IntervalsSelect-simple-select"
                value={selectedTimeInterval}
                label="Select time interval"
                onChange={handleChangeTime}
                sx={{ color: '#000000', borderColor: '#000000' }}
            >
                <MenuItem value="AllTime">From the all time</MenuItem>
                <MenuItem value="Week">From the last 7 days</MenuItem>
                <MenuItem value="Month">From the last 30 days</MenuItem>
            </Select>
        </FormControl>

        <Box display="flex" gap={1}>
            <FormControlLabel
                control={<Checkbox checked={showProviderSelect} onChange={handleCheckboxChange} />}
                label="Sort by Provider"
            />
            <FormControlLabel
                control={<Checkbox checked={!showProviderSelect} onChange={() => setShowProviderSelect(false)} />}
                label="Sort by Calendar"
            />
        </Box>

        {showProviderSelect ? (
            <FormControl fullWidth sx={{ width: '50%' }}>
                <InputLabel id="ProviderSelect" style={{ color: '#000000' }}>
                    Select provider
                </InputLabel>
                <Select
                    labelId="ProviderSelectLabel"
                    id="ProviderSelect-simple-select"
                    value={selectedProvider}
                    label="Select provider"
                    onChange={handleChangeProvider}
                    sx={{ color: '#000000', borderColor: '#000000' }}
                >
                    {providers.map((provider) => (
                        <MenuItem value={provider["id"]}>{provider["name"]}</MenuItem>

                    ))}
                </Select>
            </FormControl>
        ) : (
            <FormControl fullWidth sx={{ width: '50%' }}>
                <InputLabel id="CalendarSelect" style={{ color: '#000000' }}>
                    Select calendar
                </InputLabel>
                <Select
                    labelId="CalendarSelectLabel"
                    id="CalendarSelect-simple-select"
                    value={selectedCalendar}
                    label="Select calendar"
                    onChange={handleChangeCalendar}
                    sx={{ color: '#000000', borderColor: '#000000' }}
                >
                    {calendars.map((calendar) => (
                        <MenuItem key={calendar.id} value={calendar.id}>{calendar.name}</MenuItem>
                    ))}
                </Select>
            </FormControl>
        )}
    </Box>
    );
}

export default Selectors;
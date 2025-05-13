/**
 * @file CalendarManagementView.js
 * @description React component that renders the form interface for creating or editing calendars.
 *              It includes fields for calendar details, provider selection, duration, custom form fields,
 *              image preview, and handles user interactions such as adding/removing custom fields.
 * Author: Zverev Daniil (xzvere00)
 */

import React from 'react';
import {
    Box,
    Button,
    TextField,
    Typography,
    Select,
    MenuItem,
    Paper,
    Container,
    FormControl,
    InputLabel,
    Grid,
    Snackbar,
    Alert,
    IconButton,
    CircularProgress,
} from '@mui/material';
import { AddCircle, RemoveCircle } from '@mui/icons-material';
import PropTypes from 'prop-types';
import { format } from 'date-fns';

/**
 * CalendarManagementView Component
 *
 * @param {Object} props - Component properties.
 * @param {string} props.calendarName - The name of the calendar.
 * @param {string} props.description - Description of the calendar.
 * @param {string} props.image - URL of the calendar image.
 * @param {Array} props.providers - List of available providers.
 * @param {Array} props.selectedProviderIds - IDs of selected providers.
 * @param {number} props.duration - Duration of the calendar events in minutes.
 * @param {Array} props.formFields - Custom form fields for the calendar.
 * @param {string|Date} props.lastUpdated - Timestamp of the last update.
 * @param {boolean} props.loading - Indicates if the data is currently loading.
 * @param {boolean} props.snackbarOpen - Controls the visibility of the snackbar.
 * @param {string} props.snackbarMessage - Message to display in the snackbar.
 * @param {string} props.snackbarSeverity - Severity level of the snackbar ('success', 'error', etc.).
 * @param {string} props.error - Error message, if any.
 * @param {Function} props.handleSaveCalendar - Function to handle saving the calendar.
 * @param {Function} props.handleSnackbarClose - Function to handle closing the snackbar.
 * @param {Function} props.setCalendarName - Function to update the calendar name.
 * @param {Function} props.setDescription - Function to update the description.
 * @param {Function} props.setImage - Function to update the image URL.
 * @param {Function} props.setSelectedProviderIds - Function to update selected providers.
 * @param {Function} props.setDuration - Function to update the duration.
 * @param {Function} props.setFormFields - Function to update custom form fields.
 * @param {Function} props.handleCancel - Function to handle cancellation of the form.
 */
const CalendarManagementView = ({
                                    calendarName,
                                    description,
                                    image,
                                    providers,
                                    selectedProviderIds,
                                    duration,
                                    formFields,
                                    lastUpdated,
                                    loading,
                                    snackbarOpen,
                                    snackbarMessage,
                                    snackbarSeverity,
                                    error,
                                    handleSaveCalendar,
                                    handleSnackbarClose,
                                    setCalendarName,
                                    setDescription,
                                    setImage,
                                    setSelectedProviderIds,
                                    setDuration,
                                    setFormFields,
                                    handleCancel,
                                }) => {
    /**
     * Generates duration options in 15-minute increments up to 180 minutes.
     * @returns {Array} Array of duration option objects with value and label.
     */
    const generateDurationOptions = () => {
        const options = [];
        for (let i = 15; i <= 180; i += 15) {
            const hours = Math.floor(i / 60);
            const minutes = i % 60;
            options.push({ value: i, label: `${hours}h ${minutes}m` });
        }
        return options;
    };

    const durationOptions = generateDurationOptions();

    /**
     * Adds a new empty custom form field.
     */
    const handleAddFormField = () => {
        setFormFields([...formFields, { key: '', value: '' }]);
    };

    /**
     * Removes a custom form field at the specified index.
     * @param {number} index - Index of the form field to remove.
     */
    const handleRemoveFormField = (index) => {
        setFormFields(formFields.filter((_, i) => i !== index));
    };

    /**
     * Updates a custom form field at the specified index.
     * @param {number} index - Index of the form field to update.
     * @param {Object} field - Updated field object with key and value.
     */
    const handleChangeFormField = (index, field) => {
        const updatedFields = formFields.map((f, i) => (i === index ? field : f));
        setFormFields(updatedFields);
    };

    return (
        <Container maxWidth="md" sx={{ mt: 4 }}>
            <Paper elevation={3} sx={{ p: 4 }}>
                <Typography variant="h4">{calendarName ? 'Edit Calendar' : 'Create Calendar'}</Typography>
                {lastUpdated && (
                    <Typography variant="subtitle2" color="textSecondary">
                        Last Updated: {format(new Date(lastUpdated), 'PPP p')}
                    </Typography>
                )}
                {loading ? (
                    <Box display="flex" justifyContent="center" my={2}>
                        <CircularProgress />
                    </Box>
                ) : (
                    <>
                        {/* Calendar Name Input */}
                        <TextField
                            label="Calendar Name"
                            value={calendarName}
                            onChange={(e) => setCalendarName(e.target.value)}
                            fullWidth
                            required
                            margin="normal"
                        />
                        {/* Description Input */}
                        <TextField
                            label="Description"
                            value={description}
                            onChange={(e) => setDescription(e.target.value)}
                            fullWidth
                            multiline
                            margin="normal"
                        />
                        {/* Image URL Input */}
                        <TextField
                            label="Image URL"
                            value={image}
                            onChange={(e) => setImage(e.target.value)}
                            fullWidth
                            margin="normal"
                        />
                        {/* Image Preview */}
                        {image && (
                            <Box mt={2} textAlign="center">
                                <img src={image} alt="Calendar" style={{ maxWidth: '100%', maxHeight: 300 }} />
                            </Box>
                        )}

                        {/* Providers Selection */}
                        <FormControl fullWidth margin="normal">
                            <InputLabel>Providers</InputLabel>
                            <Select
                                multiple
                                value={selectedProviderIds}
                                onChange={(e) => setSelectedProviderIds(e.target.value)}
                                renderValue={(selected) =>
                                    selected
                                        .map((id) => {
                                            const provider = providers.find((p) => p.id === id);
                                            return provider ? provider.name : '';
                                        })
                                        .join(', ')
                                }
                            >
                                {providers.map((provider) => (
                                    <MenuItem key={provider.id} value={provider.id}>
                                        {provider.name}
                                    </MenuItem>
                                ))}
                            </Select>
                        </FormControl>

                        {/* Duration Selection */}
                        <FormControl fullWidth margin="normal">
                            <InputLabel>Duration</InputLabel>
                            <Select
                                value={duration}
                                onChange={(e) => setDuration(e.target.value)}
                                displayEmpty
                            >
                                {durationOptions.map((option) => (
                                    <MenuItem key={option.value} value={option.value}>
                                        {option.label}
                                    </MenuItem>
                                ))}
                            </Select>
                        </FormControl>

                        {/* Custom Form Fields */}
                        <Typography variant="h6" mt={3}>
                            Form Fields
                        </Typography>
                        {formFields.map((field, index) => (
                            <Grid container spacing={2} alignItems="center" key={index} mt={1}>
                                <Grid item xs={5}>
                                    <TextField
                                        label="Field Name"
                                        value={field.key}
                                        onChange={(e) =>
                                            handleChangeFormField(index, { ...field, key: e.target.value })
                                        }
                                        fullWidth
                                        required
                                    />
                                </Grid>
                                <Grid item xs={5}>
                                    <TextField
                                        label="Field Value"
                                        value={field.value}
                                        onChange={(e) =>
                                            handleChangeFormField(index, { ...field, value: e.target.value })
                                        }
                                        fullWidth
                                    />
                                </Grid>
                                <Grid item xs={2}>
                                    <IconButton
                                        color="error"
                                        onClick={() => handleRemoveFormField(index)}
                                    >
                                        <RemoveCircle />
                                    </IconButton>
                                </Grid>
                            </Grid>
                        ))}
                        <Button
                            variant="outlined"
                            startIcon={<AddCircle />}
                            onClick={handleAddFormField}
                            sx={{ mt: 2 }}
                        >
                            Add Field
                        </Button>

                        {/* Action Buttons */}
                        <Box mt={3}>
                            <Button
                                variant="contained"
                                color="primary"
                                onClick={handleSaveCalendar}
                                sx={{ mr: 2 }}
                            >
                                Save
                            </Button>
                            <Button variant="outlined" color="secondary" onClick={handleCancel}>
                                Cancel
                            </Button>
                        </Box>
                    </>
                )}
            </Paper>
            {/* Snackbar for Notifications */}
            <Snackbar
                open={snackbarOpen}
                autoHideDuration={6000}
                onClose={handleSnackbarClose}
            >
                <Alert onClose={handleSnackbarClose} severity={snackbarSeverity}>
                    {snackbarMessage}
                </Alert>
            </Snackbar>
        </Container>
    );
};

// Define prop types for the CalendarManagementView component to ensure correct usage
CalendarManagementView.propTypes = {
    /**
     * The name of the calendar.
     */
    calendarName: PropTypes.string.isRequired,

    /**
     * Description of the calendar.
     */
    description: PropTypes.string,

    /**
     * URL of the calendar image.
     */
    image: PropTypes.string,

    /**
     * List of available providers.
     */
    providers: PropTypes.array.isRequired,

    /**
     * IDs of selected providers.
     */
    selectedProviderIds: PropTypes.array.isRequired,

    /**
     * Duration of the calendar events in minutes.
     */
    duration: PropTypes.number.isRequired,

    /**
     * Custom form fields for the calendar.
     */
    formFields: PropTypes.array.isRequired,

    /**
     * Timestamp of the last update.
     */
    lastUpdated: PropTypes.oneOfType([PropTypes.string, PropTypes.instanceOf(Date)]),

    /**
     * Indicates if the data is currently loading.
     */
    loading: PropTypes.bool.isRequired,

    /**
     * Controls the visibility of the snackbar.
     */
    snackbarOpen: PropTypes.bool.isRequired,

    /**
     * Message to display in the snackbar.
     */
    snackbarMessage: PropTypes.string.isRequired,

    /**
     * Severity level of the snackbar ('success', 'error', etc.).
     */
    snackbarSeverity: PropTypes.string.isRequired,

    /**
     * Error message, if any.
     */
    error: PropTypes.string,

    /**
     * Function to handle saving the calendar.
     */
    handleSaveCalendar: PropTypes.func.isRequired,

    /**
     * Function to handle closing the snackbar.
     */
    handleSnackbarClose: PropTypes.func.isRequired,

    /**
     * Function to update the calendar name.
     */
    setCalendarName: PropTypes.func.isRequired,

    /**
     * Function to update the description.
     */
    setDescription: PropTypes.func.isRequired,

    /**
     * Function to update the image URL.
     */
    setImage: PropTypes.func.isRequired,

    /**
     * Function to update selected providers.
     */
    setSelectedProviderIds: PropTypes.func.isRequired,

    /**
     * Function to update the duration.
     */
    setDuration: PropTypes.func.isRequired,

    /**
     * Function to update custom form fields.
     */
    setFormFields: PropTypes.func.isRequired,

    /**
     * Function to handle cancellation of the form.
     */
    handleCancel: PropTypes.func.isRequired,
};

export default CalendarManagementView;

/**
 * @file ProviderProfileView.js
 * @description React component that renders the interface for displaying and managing a provider's profile.
 *              It includes functionalities such as editing provider details, managing availability,
 *              and handling user interactions like adding or removing time slots.
 * Author: Zverev Daniil (xzvere00)
 */

import React from 'react';
import {
    Box,
    TextField,
    Typography,
    Button,
    Avatar,
    Grid,
    Checkbox,
    FormControlLabel,
    IconButton,
    Snackbar,
    Alert,
    CircularProgress
} from '@mui/material';
import { Add as AddIcon, Remove as RemoveIcon } from '@mui/icons-material';
import PropTypes from 'prop-types';
import { useNavigate } from 'react-router-dom';

// Mapping of day numbers to day names for display purposes
const dayNumberToName = {
    0: 'Sunday',
    1: 'Monday',
    2: 'Tuesday',
    3: 'Wednesday',
    4: 'Thursday',
    5: 'Friday',
    6: 'Saturday'
};

/**
 * ProviderProfileView Component
 *
 * @param {Object} props - Component properties.
 * @param {string} props.firstName - Provider's first name.
 * @param {string} props.lastName - Provider's last name.
 * @param {string} props.email - Provider's email address.
 * @param {string} props.phone - Provider's phone number.
 * @param {Object} props.availability - Provider's availability schedule.
 * @param {Object} props.daysChecked - Indicates which days are selected for availability.
 * @param {boolean} props.loading - Indicates if the profile data is currently loading.
 * @param {string} props.error - Error message to display, if any.
 * @param {boolean} props.snackbarOpen - Controls the visibility of the snackbar notification.
 * @param {string} props.snackbarMessage - Message to display in the snackbar.
 * @param {string} props.snackbarSeverity - Severity level of the snackbar ('success', 'error', etc.).
 * @param {Function} props.setFirstName - Function to update the first name state.
 * @param {Function} props.setLastName - Function to update the last name state.
 * @param {Function} props.setEmail - Function to update the email state.
 * @param {Function} props.setPhone - Function to update the phone number state.
 * @param {Function} props.handleSaveProvider - Function to handle saving the provider's profile.
 * @param {Function} props.handleSnackbarClose - Function to handle closing the snackbar.
 * @param {Function} props.handleAddTimeSlot - Function to handle adding a new time slot to a day.
 * @param {Function} props.handleRemoveTimeSlot - Function to handle removing an existing time slot from a day.
 * @param {Function} props.handleTimeChange - Function to handle changes in a time slot's start or end time.
 * @param {Function} props.handleDayCheck - Function to handle checking or unchecking a day's availability.
 * @param {Function} props.handleCancel - Function to handle cancelling the profile edit.
 */
const ProviderProfileView = ({
                                 firstName,
                                 lastName,
                                 email,
                                 phone,
                                 availability,
                                 daysChecked,
                                 loading,
                                 error,
                                 snackbarOpen,
                                 snackbarMessage,
                                 snackbarSeverity,
                                 setFirstName,
                                 setLastName,
                                 setEmail,
                                 setPhone,
                                 handleSaveProvider,
                                 handleSnackbarClose,
                                 handleAddTimeSlot,
                                 handleRemoveTimeSlot,
                                 handleTimeChange,
                                 handleDayCheck,
                                 handleCancel,
                             }) => {
    const navigate = useNavigate();

    return (
        <Box p={3} sx={{ backgroundColor: '#ffffff', borderRadius: 2, boxShadow: 3 }}>
            {/* Header Section */}
            <Typography variant="h5" gutterBottom>
                {error
                    ? 'Error'
                    : firstName || lastName
                        ? 'Edit Provider'
                        : 'Add Provider'}
            </Typography>

            {/* Loading Indicator */}
            {loading && (
                <Box display="flex" justifyContent="center" my={4}>
                    <CircularProgress />
                </Box>
            )}

            {/* Profile Form */}
            {!loading && (
                <Grid container spacing={2}>
                    {/* Avatar Display */}
                    <Grid item xs={12} container justifyContent="center">
                        <Avatar sx={{ width: 100, height: 100, bgcolor: '#e0e0e0' }}>
                            {firstName ? firstName.charAt(0).toUpperCase() : '?'}
                        </Avatar>
                    </Grid>

                    {/* First Name Input */}
                    <Grid item xs={12} sm={6}>
                        <TextField
                            label="First Name"
                            value={firstName}
                            onChange={(e) => setFirstName(e.target.value)}
                            fullWidth
                            margin="normal"
                            required
                        />
                    </Grid>

                    {/* Last Name Input */}
                    <Grid item xs={12} sm={6}>
                        <TextField
                            label="Last Name"
                            value={lastName}
                            onChange={(e) => setLastName(e.target.value)}
                            fullWidth
                            margin="normal"
                            required
                        />
                    </Grid>

                    {/* Email Input */}
                    <Grid item xs={12} sm={6}>
                        <TextField
                            label="Email"
                            value={email}
                            onChange={(e) => setEmail(e.target.value)}
                            fullWidth
                            margin="normal"
                            required
                        />
                    </Grid>

                    {/* Phone Input */}
                    <Grid item xs={12} sm={6}>
                        <TextField
                            label="Phone"
                            value={phone}
                            onChange={(e) => setPhone(e.target.value)}
                            fullWidth
                            margin="normal"
                        />
                    </Grid>

                    {/* Availability Section Header */}
                    <Grid item xs={12}>
                        <Typography variant="h6" mt={3}>
                            Provider Availability
                        </Typography>
                    </Grid>

                    {/* Availability Time Slots for Each Day */}
                    {Object.keys(availability).map((day) => (
                        <Grid item xs={12} key={day}>
                            {/* Day Checkbox */}
                            <FormControlLabel
                                control={
                                    <Checkbox
                                        checked={daysChecked[day]}
                                        onChange={(e) => handleDayCheck(day, e.target.checked)}
                                        color="primary"
                                    />
                                }
                                label={dayNumberToName[day]}
                            />
                            {/* Time Slots for Checked Days */}
                            {daysChecked[day] && availability[day].map((slot, index) => (
                                <Grid container spacing={1} alignItems="center" key={index} mt={1}>
                                    {/* Start Time Input */}
                                    <Grid item>
                                        <TextField
                                            label="From"
                                            type="time"
                                            value={slot.start}
                                            onChange={(e) => handleTimeChange(day, index, 'start', e.target.value)}
                                            InputLabelProps={{ shrink: true }}
                                            inputProps={{ step: 300 }} // 5 minutes step
                                        />
                                    </Grid>
                                    {/* End Time Input */}
                                    <Grid item>
                                        <TextField
                                            label="To"
                                            type="time"
                                            value={slot.end}
                                            onChange={(e) => handleTimeChange(day, index, 'end', e.target.value)}
                                            InputLabelProps={{ shrink: true }}
                                            inputProps={{ step: 300 }} // 5 minutes step
                                        />
                                    </Grid>
                                    {/* Remove Time Slot Button */}
                                    <Grid item>
                                        <IconButton onClick={() => handleRemoveTimeSlot(day, index)} color="secondary">
                                            <RemoveIcon />
                                        </IconButton>
                                    </Grid>
                                </Grid>
                            ))}
                            {/* Add Time Slot Button */}
                            {daysChecked[day] && (
                                <Button
                                    startIcon={<AddIcon />}
                                    onClick={() => handleAddTimeSlot(day)}
                                    sx={{ mt: 1 }}
                                >
                                    Add Time Slot
                                </Button>
                            )}
                        </Grid>
                    ))}

                    {/* Display Error Message if Any */}
                    {error && (
                        <Grid item xs={12}>
                            <Typography color="error">{error}</Typography>
                        </Grid>
                    )}

                    {/* Action Buttons: Save and Cancel */}
                    <Grid item xs={12}>
                        <Button
                            variant="contained"
                            color="primary"
                            onClick={handleSaveProvider}
                            sx={{ mt: 3 }}
                        >
                            {firstName || lastName ? 'Save Changes' : 'Add Provider'}
                        </Button>
                        <Button
                            variant="outlined"
                            color="secondary"
                            onClick={handleCancel}
                            sx={{ mt: 3, ml: 2 }}
                        >
                            Cancel
                        </Button>
                    </Grid>
                </Grid>
            )}

            {/* Snackbar Notification for User Feedback */}
            <Snackbar
                open={snackbarOpen}
                autoHideDuration={6000}
                onClose={handleSnackbarClose}
                anchorOrigin={{ vertical: 'bottom', horizontal: 'center' }}
            >
                <Alert onClose={handleSnackbarClose} severity={snackbarSeverity} sx={{ width: '100%' }}>
                    {snackbarMessage}
                </Alert>
            </Snackbar>
        </Box>
    );
};

// Define prop types for the ProviderProfileView component to ensure correct usage
ProviderProfileView.propTypes = {
    /**
     * Provider's first name.
     */
    firstName: PropTypes.string.isRequired,

    /**
     * Provider's last name.
     */
    lastName: PropTypes.string.isRequired,

    /**
     * Provider's email address.
     */
    email: PropTypes.string.isRequired,

    /**
     * Provider's phone number.
     */
    phone: PropTypes.string.isRequired,

    /**
     * Provider's availability schedule.
     */
    availability: PropTypes.object.isRequired,

    /**
     * Indicates which days are selected for availability.
     */
    daysChecked: PropTypes.object.isRequired,

    /**
     * Indicates if the profile data is currently loading.
     */
    loading: PropTypes.bool.isRequired,

    /**
     * Error message to display, if any.
     */
    error: PropTypes.string,

    /**
     * Controls the visibility of the snackbar notification.
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
     * Function to update the first name state.
     */
    setFirstName: PropTypes.func.isRequired,

    /**
     * Function to update the last name state.
     */
    setLastName: PropTypes.func.isRequired,

    /**
     * Function to update the email state.
     */
    setEmail: PropTypes.func.isRequired,

    /**
     * Function to update the phone number state.
     */
    setPhone: PropTypes.func.isRequired,

    /**
     * Function to handle saving the provider's profile.
     */
    handleSaveProvider: PropTypes.func.isRequired,

    /**
     * Function to handle closing the snackbar.
     */
    handleSnackbarClose: PropTypes.func.isRequired,

    /**
     * Function to handle adding a new time slot to a day.
     */
    handleAddTimeSlot: PropTypes.func.isRequired,

    /**
     * Function to handle removing an existing time slot from a day.
     */
    handleRemoveTimeSlot: PropTypes.func.isRequired,

    /**
     * Function to handle changes in a time slot's start or end time.
     */
    handleTimeChange: PropTypes.func.isRequired,

    /**
     * Function to handle checking or unchecking a day's availability.
     */
    handleDayCheck: PropTypes.func.isRequired,

    /**
     * Function to handle cancelling the profile edit.
     */
    handleCancel: PropTypes.func.isRequired,
};

export default ProviderProfileView;

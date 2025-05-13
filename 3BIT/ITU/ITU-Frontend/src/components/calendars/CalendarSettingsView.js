/**
 * @file CalendarSettingsView.js
 * @description React component that renders the interface for managing calendar settings.
 *              It includes functionalities such as creating, editing, and deleting calendars,
 *              displaying a list of calendars, and handling user interactions through menus and dialogs.
 * Author: Zverev Daniil (xzvere00)
 */

import React from 'react';
import {
    Box,
    Button,
    Typography,
    IconButton,
    Menu,
    MenuItem,
    Table,
    TableBody,
    TableCell,
    TableContainer,
    TableHead,
    TableRow,
    Paper,
    Snackbar,
    Alert,
    CircularProgress,
    Dialog,
    DialogContent,
    DialogTitle,
} from '@mui/material';
import MoreVertIcon from '@mui/icons-material/MoreVert';
import { format } from 'date-fns';
import CalendarManagement from '../calendars/CalendarManagement';

/**
 * CalendarSettingsView Component
 *
 * @param {Object} props - Component properties.
 * @param {Array} props.calendars - List of calendar objects to display.
 * @param {boolean} props.loading - Indicates if the calendar data is currently loading.
 * @param {string} props.error - Error message to display, if any.
 * @param {HTMLElement} props.anchorEl - The current anchor element for the actions menu.
 * @param {boolean} props.snackbarOpen - Controls the visibility of the snackbar notification.
 * @param {string} props.snackbarMessage - Message to display in the snackbar.
 * @param {string} props.snackbarSeverity - Severity level of the snackbar ('success', 'error', etc.).
 * @param {Function} props.handleMenuOpen - Function to handle opening the actions menu.
 * @param {Function} props.handleMenuClose - Function to handle closing the actions menu.
 * @param {Function} props.handleEditCalendar - Function to handle editing a selected calendar.
 * @param {Function} props.handleDeleteCalendar - Function to handle deleting a selected calendar.
 * @param {Function} props.handleCreateCalendar - Function to handle initiating calendar creation.
 * @param {Function} props.handleSnackbarClose - Function to handle closing the snackbar.
 * @param {boolean} props.isModalOpen - Controls the visibility of the calendar management modal.
 * @param {string|null} props.modalCalendarId - The ID of the calendar being edited; null if creating a new calendar.
 * @param {Function} props.handleModalClose - Function to handle closing the calendar management modal.
 */
const CalendarSettingsView = ({
                                  calendars,
                                  loading,
                                  error,
                                  anchorEl,
                                  snackbarOpen,
                                  snackbarMessage,
                                  snackbarSeverity,
                                  handleMenuOpen,
                                  handleMenuClose,
                                  handleEditCalendar,
                                  handleDeleteCalendar,
                                  handleCreateCalendar,
                                  handleSnackbarClose,
                                  isModalOpen,
                                  modalCalendarId,
                                  handleModalClose,
                              }) => {
    return (
        <Box p={3} sx={{ backgroundColor: '#f9f9f9', borderRadius: 2, boxShadow: 3 }}>
            {/* Header Section */}
            <Typography variant="h4" gutterBottom>
                Calendar Settings
            </Typography>
            {/* Display error message if any */}
            {error && <Typography color="error">{error}</Typography>}

            {/* Create Calendar Button */}
            <Box display="flex" justifyContent="space-between" alignItems="center" mt={2} mb={2}>
                <Button variant="contained" color="primary" onClick={handleCreateCalendar}>
                    Create Calendar
                </Button>
            </Box>

            {/* Loading Indicator */}
            {loading ? (
                <Box display="flex" justifyContent="center" my={4}>
                    <CircularProgress />
                </Box>
            ) : (
                /* Calendars Table */
                <TableContainer component={Paper} sx={{ borderRadius: 3 }}>
                    <Table>
                        <TableHead sx={{ backgroundColor: '#f5f5f5' }}>
                            <TableRow>
                                <TableCell>Calendar</TableCell>
                                <TableCell>Providers</TableCell>
                                <TableCell>Last Edited</TableCell>
                                <TableCell align="right">Actions</TableCell>
                            </TableRow>
                        </TableHead>
                        <TableBody>
                            {calendars.map((calendar) => (
                                <TableRow key={calendar.id}>
                                    <TableCell>{calendar.name || 'Unnamed'}</TableCell>
                                    <TableCell>{calendar.providerCount}</TableCell> {/* Display provider count */}
                                    <TableCell>
                                        {calendar.updated_at
                                            ? format(new Date(calendar.updated_at), 'PPP p')
                                            : 'Not edited'}
                                    </TableCell>
                                    <TableCell align="right">
                                        {/* Actions Menu Button */}
                                        <IconButton onClick={(e) => handleMenuOpen(e, calendar)}>
                                            <MoreVertIcon />
                                        </IconButton>
                                    </TableCell>
                                </TableRow>
                            ))}
                            {/* Display message if no calendars are available */}
                            {calendars.length === 0 && (
                                <TableRow>
                                    <TableCell colSpan={4} align="center">
                                        No calendars to display.
                                    </TableCell>
                                </TableRow>
                            )}
                        </TableBody>
                    </Table>
                </TableContainer>
            )}

            {/* Actions Menu */}
            <Menu anchorEl={anchorEl} open={Boolean(anchorEl)} onClose={handleMenuClose}>
                <MenuItem onClick={handleEditCalendar}>Edit</MenuItem>
                <MenuItem onClick={handleDeleteCalendar}>Delete</MenuItem>
            </Menu>

            {/* Snackbar Notification */}
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

            {/* Modal for Creating or Editing a Calendar */}
            <Dialog
                open={isModalOpen}
                onClose={handleModalClose}
                fullWidth
                maxWidth="md"
            >
                <DialogTitle>{modalCalendarId ? 'Edit Calendar' : 'Create Calendar'}</DialogTitle>
                <DialogContent>
                    <CalendarManagement calendarId={modalCalendarId} onClose={handleModalClose} />
                </DialogContent>
            </Dialog>
        </Box>
    );
};

export default CalendarSettingsView;

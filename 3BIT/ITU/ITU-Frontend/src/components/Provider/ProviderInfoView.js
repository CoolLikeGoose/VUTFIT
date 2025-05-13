/**
 * @file ProviderInfoView.js
 * @description React component that renders the interface for displaying and managing provider information.
 *              It includes functionalities such as adding, editing, and deleting providers,
 *              searching through providers, and displaying provider details in a structured table.
 * Author: Zverev Daniil (xzvere00)
 */

import React from 'react';
import {
    Box,
    Button,
    TextField,
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
    Avatar,
    Stack,
    Snackbar,
    Alert,
    Dialog,
    DialogContent,
    DialogTitle,
} from '@mui/material';
import MoreVertIcon from '@mui/icons-material/MoreVert';
import { Add as AddIcon } from '@mui/icons-material';
import PropTypes from 'prop-types';
import ProviderProfile from './ProviderProfile';
import { format } from 'date-fns';

/**
 * ProviderInfoView Component
 *
 * @param {Object} props - Component properties.
 * @param {Array} props.providers - List of provider objects to display.
 * @param {string} props.searchTerm - Current search term for filtering providers.
 * @param {HTMLElement} props.anchorEl - The current anchor element for the actions menu.
 * @param {Object} props.selectedProvider - The provider object currently selected for actions.
 * @param {string} props.error - Error message to display, if any.
 * @param {boolean} props.snackbarOpen - Controls the visibility of the snackbar notification.
 * @param {string} props.snackbarMessage - Message to display in the snackbar.
 * @param {string} props.snackbarSeverity - Severity level of the snackbar ('success', 'error', etc.).
 * @param {Function} props.setSearchTerm - Function to update the search term state.
 * @param {Function} props.handleMenuOpen - Function to handle opening the actions menu.
 * @param {Function} props.handleMenuClose - Function to handle closing the actions menu.
 * @param {Function} props.handleEditProvider - Function to handle editing a selected provider.
 * @param {Function} props.handleCreateProvider - Function to handle initiating the creation of a new provider.
 * @param {Function} props.handleDeleteProvider - Function to handle deleting a selected provider.
 * @param {Function} props.handleSnackbarClose - Function to handle closing the snackbar.
 * @param {boolean} props.isModalOpen - Controls the visibility of the provider management modal.
 * @param {string|null} props.modalProviderId - The ID of the provider being edited; null if creating a new provider.
 * @param {Function} props.handleModalClose - Function to handle closing the provider management modal.
 */
const ProviderInfoView = ({
                              providers,
                              searchTerm,
                              anchorEl,
                              selectedProvider,
                              error,
                              snackbarOpen,
                              snackbarMessage,
                              snackbarSeverity,
                              setSearchTerm,
                              handleMenuOpen,
                              handleMenuClose,
                              handleEditProvider,
                              handleCreateProvider,
                              handleDeleteProvider,
                              handleSnackbarClose,
                              isModalOpen,
                              modalProviderId,
                              handleModalClose,
                          }) => {
    return (
        <Box p={3} sx={{ backgroundColor: '#f9f9f9', borderRadius: 2, boxShadow: 3 }}>
            {/* Header Section */}
            <Typography variant="h4" gutterBottom>
                Provider Profiles
            </Typography>
            {/* Display error message if any */}
            {error && <Typography color="error">{error}</Typography>}

            {/* Actions: Add Provider Button and Search Field */}
            <Box display="flex" justifyContent="space-between" alignItems="center" mt={2} mb={2}>
                <Button
                    variant="contained"
                    color="success"
                    startIcon={<AddIcon />}
                    onClick={handleCreateProvider}
                >
                    Add Provider
                </Button>
                <TextField
                    label="Search Providers"
                    value={searchTerm}
                    onChange={(e) => setSearchTerm(e.target.value)}
                    variant="outlined"
                    size="small"
                />
            </Box>

            {/* Providers Table */}
            <TableContainer component={Paper} sx={{ borderRadius: 3 }}>
                <Table>
                    {/* Table Header */}
                    <TableHead sx={{ backgroundColor: '#f5f5f5' }}>
                        <TableRow>
                            <TableCell>Provider</TableCell>
                            <TableCell>Created At</TableCell>
                            <TableCell>Calendars</TableCell>
                            <TableCell align="right">Actions</TableCell>
                        </TableRow>
                    </TableHead>
                    <TableBody>
                        {/* Render providers if available */}
                        {providers.length > 0 ? (
                            providers
                                .filter((provider) =>
                                    (provider.firstName && provider.firstName.toLowerCase().includes(searchTerm.toLowerCase())) ||
                                    (provider.lastName && provider.lastName.toLowerCase().includes(searchTerm.toLowerCase()))
                                )
                                .map((provider) => (
                                    <TableRow key={provider.id}>
                                        {/* Provider Information */}
                                        <TableCell>
                                            <Stack direction="row" spacing={2} alignItems="center">
                                                <Avatar>
                                                    {provider.firstName
                                                        ? provider.firstName.charAt(0).toUpperCase()
                                                        : '?'}
                                                </Avatar>
                                                <Typography>
                                                    {provider.firstName || ''} {provider.lastName || ''}
                                                </Typography>
                                            </Stack>
                                        </TableCell>
                                        {/* Provider Creation Date */}
                                        <TableCell>
                                            {provider.createdAt
                                                ? format(new Date(provider.createdAt), 'PPP p')
                                                : 'N/A'}
                                        </TableCell>
                                        {/* Number of Calendars Associated with Provider */}
                                        <TableCell>{provider.calendarCount}</TableCell>
                                        {/* Actions Menu */}
                                        <TableCell align="right">
                                            <IconButton onClick={(e) => handleMenuOpen(e, provider)}>
                                                <MoreVertIcon />
                                            </IconButton>
                                        </TableCell>
                                    </TableRow>
                                ))
                        ) : (
                            // Display message if no providers are available
                            <TableRow>
                                <TableCell colSpan={4} align="center">
                                    No providers to display.
                                </TableCell>
                            </TableRow>
                        )}
                    </TableBody>
                </Table>
            </TableContainer>

            {/* Actions Menu for Each Provider */}
            <Menu anchorEl={anchorEl} open={Boolean(anchorEl)} onClose={handleMenuClose}>
                <MenuItem onClick={handleEditProvider}>Edit</MenuItem>
                <MenuItem onClick={handleDeleteProvider}>Delete</MenuItem>
            </Menu>

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

            {/* Modal Dialog for Adding or Editing a Provider */}
            <Dialog open={isModalOpen} onClose={handleModalClose} fullWidth maxWidth="md">
                <DialogTitle>{modalProviderId ? 'Edit Provider' : 'Add Provider'}</DialogTitle>
                <DialogContent>
                    <ProviderProfile providerId={modalProviderId} onClose={handleModalClose} />
                </DialogContent>
            </Dialog>
        </Box>
    );
};

// Define prop types for the ProviderInfoView component to ensure correct usage
ProviderInfoView.propTypes = {
    /**
     * List of provider objects to display.
     */
    providers: PropTypes.array.isRequired,

    /**
     * Current search term for filtering providers.
     */
    searchTerm: PropTypes.string.isRequired,

    /**
     * The current anchor element for the actions menu.
     */
    anchorEl: PropTypes.any,

    /**
     * The provider object currently selected for actions.
     */
    selectedProvider: PropTypes.object,

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
     * Function to update the search term state.
     */
    setSearchTerm: PropTypes.func.isRequired,

    /**
     * Function to handle opening the actions menu.
     */
    handleMenuOpen: PropTypes.func.isRequired,

    /**
     * Function to handle closing the actions menu.
     */
    handleMenuClose: PropTypes.func.isRequired,

    /**
     * Function to handle editing a selected provider.
     */
    handleEditProvider: PropTypes.func.isRequired,

    /**
     * Function to handle initiating the creation of a new provider.
     */
    handleCreateProvider: PropTypes.func.isRequired,

    /**
     * Function to handle deleting a selected provider.
     */
    handleDeleteProvider: PropTypes.func.isRequired,

    /**
     * Function to handle closing the snackbar.
     */
    handleSnackbarClose: PropTypes.func.isRequired,

    /**
     * Controls the visibility of the provider management modal.
     */
    isModalOpen: PropTypes.bool.isRequired,

    /**
     * The ID of the provider being edited; null if creating a new provider.
     */
    modalProviderId: PropTypes.string,

    /**
     * Function to handle closing the provider management modal.
     */
    handleModalClose: PropTypes.func.isRequired,
};

export default ProviderInfoView;

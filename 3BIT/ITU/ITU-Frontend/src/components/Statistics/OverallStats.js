import React, { useState } from 'react';
import {
    Box,
    Paper,
    Typography
} from '@mui/material';
import { useNavigate } from 'react-router-dom';

const OverallStats = () => {
    const [totalBookings, setTotalBookings] = useState(0);
    const [availabilityPercent, setAvailabilityPercent] = useState(0);
    const [completedAppointments, setCompletedAppointments] = useState(0);
    const [cancelledAppointments, setCancelledAppointments] = useState(0);
    const [error, setError] = useState(null);
    const navigate = useNavigate();

    return (
        <Box
            display="flex"
            justifyContent="center"
            alignItems="center"
            flexDirection="column"
            height="70vh"
        >
            <Typography variant="h5" sx={{ marginBottom: 4 }}>
                Overall statistics
            </Typography>

            <Box
                display="grid"
                gridTemplateColumns="repeat(2, 1fr)"
                gap={4}
                sx={{
                    width: '80%',
                    maxWidth: '900px',
                }}
            >
                <Paper
                    sx={{
                        display: 'flex',
                        justifyContent: 'center',
                        alignItems: 'center',
                        height: 150,
                        backgroundColor: '#f6ebfa',
                        boxShadow: 3,
                        borderRadius: 2,
                    }}
                >
                    <Typography variant="h6" color="black">
                        {`Total bookings: ${totalBookings}`}
                    </Typography>
                </Paper>

                <Paper
                    sx={{
                        display: 'flex',
                        justifyContent: 'center',
                        alignItems: 'center',
                        height: 150,
                        backgroundColor: '#f6ebfa',
                        boxShadow: 3,
                        borderRadius: 2,
                    }}
                >
                    <Typography variant="h6" color="black">
                        {`Availability percent: ${availabilityPercent} %`}
                    </Typography>
                </Paper>

                <Paper
                    sx={{
                        display: 'flex',
                        justifyContent: 'center',
                        alignItems: 'center',
                        height: 150,
                        backgroundColor: '#f6ebfa',
                        boxShadow: 3,
                        borderRadius: 2,
                    }}
                >
                    <Typography variant="h6" color="black">
                        {`Completed appointments: ${completedAppointments}`}
                    </Typography>
                </Paper>

                <Paper
                    sx={{
                        display: 'flex',
                        justifyContent: 'center',
                        alignItems: 'center',
                        height: 150,
                        backgroundColor: '#f6ebfa',
                        boxShadow: 3,
                        borderRadius: 2,
                    }}
                >
                    <Typography variant="h6" color="black">
                        {`Cancelled appointments: ${cancelledAppointments}`}
                    </Typography>
                </Paper>
            </Box>
        </Box>
    );
}
export default OverallStats;
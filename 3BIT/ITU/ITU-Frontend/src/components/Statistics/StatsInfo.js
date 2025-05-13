import React, { useState } from 'react';
import {
    Box,
    Typography,
} from '@mui/material';
import { useNavigate } from 'react-router-dom';
import Selectors from  '../Statistics/Selectors';
import OverallStats from "./OverallStats";
import Graphs from "./Graphs";


const StatsInfo = () => {
    const [error, setError] = useState(null);
    const navigate = useNavigate();

    return (
        <Box p={3} sx={{ backgroundColor: '#f9f9f9', borderRadius: 2, boxShadow: 3 }}>
            <Typography variant="h6" style={{ marginTop: '20px', marginBottom: '10px', textAlign: 'center' }}>
                Statistics
            </Typography>
            <Selectors/>
            <OverallStats/>
            <Graphs/>
        </Box>
    );
};

export default StatsInfo;

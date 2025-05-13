import React from 'react';
import { Container, Typography, Box, Button } from '@mui/material';
import StatsInfo from '../components/Statistics/StatsInfo';
import { useNavigate } from 'react-router-dom';

const StatsPage = () => {
    // const navigate = useNavigate();
    // const handleAddProvider = () => {
    //     navigate('/stats-info');
    // };

    return (
        <Container>
            <StatsInfo />
        </Container>
    );
};

export default StatsPage;
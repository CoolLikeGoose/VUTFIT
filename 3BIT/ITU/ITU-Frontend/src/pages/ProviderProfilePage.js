// src/pages/ProviderProfilePage.js

import React from 'react';
import { Container } from '@mui/material';
import ProviderProfile from '../components/Provider/ProviderProfile';
import { useNavigate } from 'react-router-dom';

const ProviderProfilePage = () => {
    const navigate = useNavigate();

    const handleCancel = () => {
        navigate('/provider-info');
    };

    return (
        <Container>
            <ProviderProfile onClose={handleCancel} />
        </Container>
    );
};

export default ProviderProfilePage;

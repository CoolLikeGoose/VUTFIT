import React, { useState } from 'react';
import {
    Box,
    TextField,
    Typography
} from '@mui/material';
import LineChart from '../Statistics/LineChart';
import BarChart from '../Statistics/BarChart';

const Graphs = (props) => {
    const [threshold, setThreshold] = useState(0);

    const data = [
        { date: new Date(2023, 0, 1), value: 30 },
        { date: new Date(2023, 1, 1), value: 40 },
        { date: new Date(2023, 2, 1), value: 35 },
        { date: new Date(2023, 3, 1), value: 50 },
        { date: new Date(2023, 4, 1), value: 45 },
        { date: new Date(2023, 5, 1), value: 60 },
    ];
    const data1 = [
        { name: 'A', value: 30 },
        { name: 'B', value: 80 },
        { name: 'C', value: 45 },
        { name: 'D', value: 60 },
        { name: 'E', value: 20 },
        { name: 'F', value: 90 },
        { name: 'G', value: 55 },
    ];

    const handleThresholdSet = (event) => {
        setThreshold(event.target.value);
    };



    return (
        <Box>
            <Box display="flex" flexDirection="row" alignItems="center" gap={3}>
                <Typography variant="h5" color="black" style={{ marginTop: '20px', marginBottom: '10px', textAlign: 'left' }}>
                    Booking trends
                </Typography>
                <TextField
                    label="Set threshold"
                    singleline
                    value={threshold}
                    onChange={handleThresholdSet}
                    variant="outlined"
                    fullWidth
                    sx={{ maxWidth: '400px',maxHeight: '40px' }}
                />
                <Typography variant="h5" color="black" style={{ marginTop: '20px', marginBottom: '10px', textAlign: 'left' }}>
                    Provider performance
                </Typography>
                <TextField
                    label="Set threshold"
                    singleline
                    value={threshold}
                    onChange={handleThresholdSet}
                    variant="outlined"
                    fullWidth
                    sx={{ maxWidth: '400px',maxHeight: '40px' }}
                />
            </Box>
            <Box display="flex" flexDirection="row" gap={2} justifyContent="center" alignItems="center" width="100%">
                <Box flex={1} maxWidth="50%">
                    <LineChart data={data} width={550} height={500}/>
                </Box>
                <Box flex={1} maxWidth="50%">
                    <BarChart data={data1} width={550} height={500}/>
                </Box>
            </Box>
        </Box>
    );
}
export default Graphs;
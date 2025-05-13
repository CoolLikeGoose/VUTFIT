import React, { useEffect, useState } from 'react';
import LineChart from '../Statistics/LineChart';
import { appointmentsService } from '../../services/appointmentsService';

const processData = (appointments) => {
    const result = {};

    appointments.forEach(appointment => {
        const date = new Date(appointment.appointment_datetime).toISOString().split('T')[0];
        if (!result[date]) {
            result[date] = 0;
        }
        result[date] += 1;
    });

    const data = Object.keys(result).map(date => ({
        date: new Date(date),
        value: result[date]
    }));

    return data;
};

const AppointmentChart = ({ width = 600, height = 400 }) => {
    const [appointmentsData, setAppointmentsData] = useState([]);
    const [error, setError] = useState(null);

    useEffect(() => {
        const loadAppointments = async () => {
            try {

                const endDate = new Date();
                const startDate = new Date();
                startDate.setMonth(startDate.getMonth() - 1);

                const start = startDate.toISOString().split('T')[0];
                const end = endDate.toISOString().split('T')[0];

                const appointments = await appointmentsService.getAppointments(start, end, [], null);

                const data = processData(appointments);
                setAppointmentsData(data);
            } catch (err) {
                setError('Failed to load appointments');
                console.error(err);
            }
        };

        loadAppointments();
    }, []);

    if (error) {
        return <div>Error: {error}</div>;
    }

    return (
        <LineChart data={appointmentsData} width={width} height={height} />
    );
};

export default AppointmentChart;
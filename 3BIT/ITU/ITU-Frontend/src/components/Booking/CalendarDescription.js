/**
 * @file CalendarDescription.js
 * @description Contains the minimum necessary information regarding the current calendar,
 * so that the user can decide which date suits him best. It also contains a field for selecting the preferred provider. 
 * @author Nikita Smirnov (xsmirn02)
 */

import React, { useEffect, useState } from 'react';
import "../../styles/booking_page.css"

export const CalendarDescription = ({eventName, eventImg, details,
     duration, place, avaibleProviders, setSelectedProvider, appointmentProviderID}) => {

    const [displayedProvider, setDisplayedProvider] = useState('');
    useEffect(() => {
        if (appointmentProviderID) {
            const selectedProvider = avaibleProviders.find(provider => provider.id == appointmentProviderID);
            setDisplayedProvider(selectedProvider.name);
        } else {
            setDisplayedProvider('');
        }
    }, [appointmentProviderID]);

    const handleProviderChange = (event) => {
        const newProvider = event.target.value;
        setSelectedProvider(newProvider);
    };

    return (
        <div className='description'> 
            <img src={eventImg}/>
            <div className='field'>
                <span className='label'>Event</span>
                <span className='value name'> {eventName}</span>
            </div>
            <div className='field'>
                <span className='label'>Details</span>
                <span className='value'>{details}</span>
            </div>
            <div className='field'>
                <span className='label'>Duration</span>
                <span className='value'>{duration} min</span>
            </div>
            <div className='field'>
                <span className='label'>Place</span>
                <span className='value'>{place}</span>
            </div>
            <div className='field'>
                <span className='label'>Practitioner</span> 
                {appointmentProviderID == null ? (
                    <select className='select' onChange={handleProviderChange}>
                    <option value="">Any</option>
                    {avaibleProviders.map((provider) => (
                        <option key={provider['id']} value={provider['id']}>{provider['name']}</option>
                    ))}
                </select>
                ) : (
                    <span className='value'>{displayedProvider}</span>
                )}
            </div>
        </div> 
    );
};

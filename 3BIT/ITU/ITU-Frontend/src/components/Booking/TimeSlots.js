/**
 * @file TimeSlots.js
 * @description Provides the user with a minimalistic and user-friendly way to choose the right time to register.
 * @author Nikita Smirnov (xsmirn02)
 */

import React, {useState, useEffect} from 'react';
import '../../styles/booking_page.css';

export const TimeSlots = ({ formatedDate, setSelectedTime, timeSlots, setIsConfirmed, setAppointmentProviderID }) => {
    const [expandedButton, setExpandedButton] = useState(null);

    const handleButtonClick = (time) => {
      setExpandedButton(time === expandedButton ? null : time);
    };

    const handleConfirm = () => {
      if (expandedButton) {
        setSelectedTime(expandedButton);
        setIsConfirmed(true);
        setExpandedButton(null);
        
        //set random provider
        const providers = timeSlots.find(([slotTime]) => slotTime === expandedButton)[1];
        const randomIndex = Math.floor(Math.random() * providers.length);
        setAppointmentProviderID(providers[randomIndex]);
      }
    };

    useEffect(() => {
        setExpandedButton(null);
    }, [timeSlots])

    return (
        <div className="time-slots">
            <div className='time-header'>{formatedDate}</div>
            {timeSlots.length > 0 ? (
                <div className='scrollable-container'>
                    <div className='btn-list'> 
                        {timeSlots.map(([time, providers]) => (
                            <div className='btn-wrapper' key={providers[0]+time}>
                            {expandedButton == time ? (
                                <div className='btn-expanded'> 
                                    <button className='expanded-btn-time' onClick={() => handleButtonClick(time)}>{time}</button>

                                    <button className="btn-confirm" onClick={handleConfirm}>Confirm</button>
                                </div> 
                            ) : (
                                <button className="btn-time" onClick={() => handleButtonClick(time)}>
                                    {time}
                                </button>
                            )}
                            </div>
                        ))}
                    </div>
                </div>
            ) : (
                <p>No time slots available for this date.</p>
            )}
        </div>
    );
};
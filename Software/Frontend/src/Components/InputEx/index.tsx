import React, { KeyboardEvent, useEffect, useState } from 'react';

interface IInputExProps {
    onCommitValue: (value: string) => void;
    initialValue: string;
}

const InputEx = ({ onCommitValue, initialValue }: IInputExProps) => {
    const [currentValue, setCurrentValue] = useState(initialValue);
    const handleKeyDown = (event: KeyboardEvent<HTMLInputElement>) => {
        if (event.key === 'Enter' && onCommitValue) {
            onCommitValue(currentValue);
        }
    };

    return (
        <input
            type="text"
            onKeyDown={handleKeyDown}
            value={currentValue}
            onChange={(event) => setCurrentValue(event.target.value)}
            onBlur={(event) => onCommitValue(currentValue)}
        />
    );
};

export default InputEx;

% Configure Serial Port
%IT IS IMPORTANT TO USE "CLC" & "CLEAR" in the command window before
%running this code
% Make sure the Arduino software has been closed, make sure serialPort is
% the same as Arduino and BaudRate is the same as Arduino code
serialPort = "COM11"; 
baudRate = 9600;
serialObj = serialport(serialPort, baudRate);

% Initialize variables
bufferSize = 100; % Number of data points to display in the live plot
data = nan(bufferSize, 3); % Buffer to store temperatures (Hot Plate, Guard, Cold Plate)

% Set up live plot
figure;
hotLine = animatedline('Color', 'r', 'LineWidth', 1.5, 'DisplayName', 'Hot Plate');
guardLine = animatedline('Color', 'g', 'LineWidth', 1.5, 'DisplayName', 'Guard');
coldLine = animatedline('Color', 'b', 'LineWidth', 1.5, 'DisplayName', 'Cold Plate');
legend show;
xlabel('Time (s)');
ylabel('Temperature (°C)');
title('Live Temperature Data');
grid on;

% Timing for live plot
startTime = datetime('now');

try
    while true
        % Read a line from the serial port
        line = readline(serialObj);
        
        % Parse the CSV data
        values = str2double(strsplit(line, ','));
        if length(values) == 3 % Ensure valid data is received
            % Update the buffer
            data = [data(2:end, :); values]; % Shift buffer and append new data
            
            % Update the live plot
            currentTime = seconds(datetime('now') - startTime);
            addpoints(hotLine, currentTime, values(1));
            addpoints(guardLine, currentTime, values(2));
            addpoints(coldLine, currentTime, values(3));
            drawnow;
        end
    end
catch ME

    % Clean up
    clear serialObj;
    rethrow(ME);
end

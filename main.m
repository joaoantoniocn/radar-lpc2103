%run('clean');
clear all;
close all;
 
s = serial('COM1'); %assigns the object s to serial port
 
set(s, 'InputBufferSize', 256); %number of bytes in inout buffer
set(s, 'FlowControl', 'hardware');
set(s, 'BaudRate', 9600);
set(s, 'Parity', 'none');
set(s, 'DataBits', 8);
set(s, 'StopBit', 1);
set(s, 'Timeout',4);
%clc;
 
disp(get(s,'Name'));
prop(1)=(get(s,'BaudRate'));
prop(2)=(get(s,'DataBits'));
prop(3)=(get(s, 'StopBit'));
prop(4)=(get(s, 'InputBufferSize'));
 
disp(['Port Setup Done!!',num2str(prop)]);
 
fopen(s);           %opens the serial port
t=1;
disp('Running');
y=0;
x = 0;
while(t < 100)  %Runs for 200 cycles - if you cant see the symbol, it is "less than" sign. so while (t less than 200)
    a =fread(s); %reads the data from the serial port and stores it to the matrix a
%    a=max(a);  % in this particular example, I'm plotting the maximum value of the 256B input buffer
%  
%     disp(a);
%     if a == [] 
%        disp('fudeu');
%        a = 0;
%     end    
     y =[y a];       % Merging the value to an array, this is not very computationaly effective, as the array size is dynamic.
%     x = [x t];    
%                  %Consider pre allocation the size of the array to avoid this. But beware, You might loose some important
%                   %data at the end!
%  
    
%     stem(x, y, 'filled');
    plot(y,'--go',...
    'LineWidth',2,...
    'MarkerSize',10,...
    'MarkerEdgeColor','b',...
    'MarkerFaceColor',[0.5,0.5,0.5]);
    axis auto;
     grid on;
 
    disp(num2str(a));
    hold on;
   t=t+1;
   a=0;  %Clear the buffer
    drawnow;
end
 
fclose(s); %close the serial port
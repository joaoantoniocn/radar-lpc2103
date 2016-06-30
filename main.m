%run('clean');
clear all;
close all;

s = serial('COM1'); % instancia a porta serial COM1

set(s, 'InputBufferSize', 256); % numero de bytes no buffer
set(s, 'FlowControl', 'hardware');
set(s, 'BaudRate', 9600); % taxa de transmissao
set(s, 'Parity', 'none');
set(s, 'DataBits', 8);
set(s, 'StopBit', 1);
set(s, 'Timeout',4); % tempo que vai esperar ate olhar os dados da porta serial de novo
%clc;

% printando configuracao da porta serial
disp(get(s,'Name'));
prop(1)=(get(s,'BaudRate'));
prop(2)=(get(s,'DataBits'));
prop(3)=(get(s, 'Timeout'));
prop(4)=(get(s, 'InputBufferSize'));

disp(['Port Setup Done!! ',num2str(prop)]);

fopen(s);           % abre a porta serial
t=1;
disp('Running');
y=0;

while(t < 100)  % numero de vezes que quero ler da serial
    a =fread(s); % lendo os dados da serial
    
    y =[y a];       % salvando os valores de `a` no array `y`
    
    % mantem somente os 10 ultimos valores recebidos
    if length(y) > 10
       y(1) = []; 
    end    
    
    plot(y,'--go',...
        'LineWidth',2,...
        'MarkerSize',10,...
        'MarkerEdgeColor','b',...
        'MarkerFaceColor',[0.5,0.5,0.5]);
    axis auto;
    grid on;
    
    % printando o valor recebido
    disp(a);
    hold on;
    t=t+1;
    a=0;  % limpando valor de `a`
    drawnow;
end

fclose(s); % fechando a porta serial
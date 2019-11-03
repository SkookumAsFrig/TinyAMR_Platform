clc
clear all
close all
sys_clock = 16000000;
presclar1 = 1;
presclar2 = 8;
presclar3 = 64;
presclar4 = 256;
presclar5 = 1024;
topPlus = linspace(1, 35000, 35000);
y = zeros(5,35000);
delta = zeros(1, 5);
F = linspace(1, 35000, 35000);
actualF = inf(35000,2);
scal1 = [];
scal2 = [];
scal3 = [];
scal4 = [];
scal5 = [];
for i = 1 : 35000
    y(1,i) = round(sys_clock / (2 * presclar1 * F(i)));
    if y(1,i)>2^16
        y(1,i)=0;
    end
    y(2,i) = round(sys_clock / (2 * presclar2 * F(i)));
    if y(2,i)>2^16
        y(2,i)=0;
    end
    y(3,i) = round(sys_clock / (2 * presclar3 * F(i)));
    if y(3,i)>2^16
        y(3,i)=0;
    end
    y(4,i) = round(sys_clock / (2 * presclar4 * F(i)));
    if y(4,i)>2^16
        y(4,i)=0;
    end
    y(5,i) = round(sys_clock / (2 * presclar5 * F(i)));
    if y(5,i)>2^16
        y(5,i)=0;
    end
    delta(1) = abs(F(i) - sys_clock / (2 * presclar1 * y(1, i)));
    delta(2) = abs(F(i) - sys_clock / (2 * presclar2 * y(2, i)));
    delta(3) = abs(F(i) - sys_clock / (2 * presclar3 * y(3, i)));
    delta(4) = abs(F(i) - sys_clock / (2 * presclar4 * y(4, i)));
    delta(5) = abs(F(i) - sys_clock / (2 * presclar5 * y(5, i)));
    [minvalue, minidx] = min(delta);
    actualF(i,1) = minidx;
    if minidx == 1
        scal1 = [scal1; [i, minvalue]];
    elseif minidx == 2
        scal2 = [scal2; [i, minvalue]];
    elseif minidx == 3
        scal3 = [scal3; [i, minvalue]];
    elseif minidx == 4
        scal4 = [scal4; [i, minvalue]];
    else
        scal5 = [scal5; [i, minvalue]];
    end
    actualF(i,2) = minvalue;
end

figure
hold on
plot(scal1(:,1),scal1(:,2), '.')
plot(scal2(:,1),scal2(:,2), '.')
plot(scal3(:,1),scal3(:,2), '.')
plot(scal4(:,1),scal4(:,2), '.')





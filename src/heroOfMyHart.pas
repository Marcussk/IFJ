var
	lines : integer;
	delayStrength : integer; 
	res :boolean;	
	waited : boolean;
function delay(ms: integer) : boolean;
var
	i : integer;
	i2 : integer;
begin
	i:= 0;
	i2:= 0;
	while ms >0 do
	begin
		i := 0;
		while i < delayStrength do
		begin
			i:= i +1
		end;
		ms := ms -1
	end;
	delay := true
end;

function newlines(n : integer) : boolean;
var
	i : integer;
begin
	i:= n;
	while i > 0 do
	begin
		write(''#10'');
 		i :=i -1
	end;	
	newlines := true
end;

function writeMsg(str : string; ms : integer) : boolean;
var 
 res : boolean;
begin
	res:= newlines(10);
	write(str);
	res:= newlines(13);
	res := delay(ms);
	writeMsg:= res
end;

function screenClr(ms : integer) : boolean;
begin
	screenClr:= newlines(lines)
end;

function loading(ms : integer) : boolean;
var
	res : boolean;
begin
	res := writeMsg('Loading…'#10'█▒▒▒▒▒▒▒▒▒', ms);
	res := writeMsg('10%'#10'███▒▒▒▒▒▒▒', ms);
	res := writeMsg('30%'#10'█████▒▒▒▒▒', ms);
	res := writeMsg('50%'#10'███████▒▒▒', ms);
	res := writeMsg('90%'#10'█████████▒', ms);
	res := writeMsg('90%'#10'█████████▒', ms);
	res := writeMsg('90%'#10'█████████▒', ms);
	res := writeMsg('Loading…'#10'█▒▒▒▒▒▒▒▒▒', ms);
	res := writeMsg('FUCKED%'#10'█████████▒', 1000);
	loading := res
end;

function writeEpicFail(ms:integer) : boolean;
begin
	write('░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░'#10'░█▀▀ ░█▀█ ░█ ░█▀▀ ░░█▀▀ ░█▀█░█ ░█░░░░'#10'░█▀▀ ░█▀▀ ░█ ░█ ░░░░█▀▀ ░█▀█░█ ░█ ░░░░'#10'░▀▀▀ ░▀ ░░░▀ ░▀▀▀ ░░▀░░ ░▀░▀░▀ ░▀▀▀░░')
end;

begin
	lines := 24;
	delayStrength := 5000;
	res := loading(100);
	res :=writeMsg('Like boss, i was failing', 3000);
	res :=writeMsg('Now is time for the retribution', 3000);
	res :=writeMsg('One day, in the area Bozetechova, 5 peoples ready for the deaths stend in front of the dors.'#10'In front of the dors of theyrs destiny, infront of the pure evil', 3000);
	res :=writeMsg('15 mins. later:', 1500);
	res :=delay(3000)
end.

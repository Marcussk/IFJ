var
	num1 : integer;
	num2 : integer;
	num3 : integer;
	maxresult : integer;
	factresult : integer;
	
function factorial(n : integer) : integer; forward;
	
function max(val1 : integer; val2 : integer; val3 : integer) : integer;
var 
	result : integer;
	
begin
	result := val1;
	if (val2 >= result) then begin
		result := val2
	end
	else begin
	end;
	
	if (val3 >= result) then begin
		result := val3
	end
	else begin
	end;
	
	max := result
end;

function factorial(n : integer) : integer;
var
    temp_result : integer;
    decremented_n : integer;
begin
    if n < 2 then
    begin
        factorial := 1
    end
    else
    begin
        decremented_n := n - 1;
        temp_result := factorial(decremented_n);
        factorial := n * temp_result
    end
end;

{ Hlavni telo }
begin
	num1 := 1;
	num2 := 2;
	num3 := 3;
	
	maxresult := max(num1, num2, num3);
	factresult := factorial(3);
	write('Nejvetsi cislo je: ', maxresult, ''#10'');
	write('Factorial je: ', factresult)
end.

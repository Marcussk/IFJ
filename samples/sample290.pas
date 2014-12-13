{ write string + string }
{ Hlavni telo programu }
var
	res : string;
	operand1 : string;
	operand2 : string;
	i :integer;
begin
operand1 := 'Konka';
	operand2 := 'tenace';
	i:=0;
	res := operand1 + operand2;
	while i < 10 do
	begin	
 		res := res + operand1 + operand2;
		i:= i+1 
	end;
    	write(res)
end.

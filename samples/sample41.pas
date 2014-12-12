{ write false = true }
{ Hlavni telo programu }
var
	res : boolean;
	operand1 : boolean;
	operand2 : boolean;
begin
	operand1 := false;
	operand2 := true;
    res := operand1 = operand2;
    write(res)
end.

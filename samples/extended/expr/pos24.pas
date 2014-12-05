{ write int }
{ Hlavni telo programu }
var
	res : boolean;
	operand1 : real;
	operand2 : real;
begin
	operand1 := 3.1415926;
	operand2 := 6e2;
    res := operand1 <> operand2;
    write(res)
end.

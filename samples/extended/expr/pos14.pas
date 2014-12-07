{ write string <= string }
{ Hlavni telo programu }
var
	res : boolean;
	operand1 : string;
	operand2 : string;
begin
	operand1 := 'Konka';
	operand2 := 'tenace';
    res := operand1 <= operand2;
    write(res)
end.

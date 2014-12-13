{ nedochadza k pretypovaniu string real}
var
	a : integer;
begin 
	a := 5 ;
	repeat
	begin
		write(a);
		a := a - 1
	end
	until a > 0 ;
	write('vysl:', a, ''#10'')
end.	
	vysl : string;
	assign : real;
{ Hlavni telo programu }
begin
	assign := 3.1415926;
	vysl := assign;
    write(vysl)
end.

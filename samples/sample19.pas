{ nedochadza k pretypovaniu string real}
var
<<<<<<< HEAD
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
=======
	vysl : string;
	assign : real;
{ Hlavni telo programu }
begin
	assign := 3.1415926;
	vysl := assign;
    write(vysl)
end.
>>>>>>> 7934910f377e6d3f40796e152699f55b570a1cea

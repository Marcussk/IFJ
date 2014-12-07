{ Jednoduchy test na repat-until}
var
	a : integer;
begin
	a := 5 ;
	repeat
	begin
		write(a);
		a := a - 1
	end
	until a < 0 
	write('vysl:', a, ''#10'')
end.	

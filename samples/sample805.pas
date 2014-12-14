{ FOR uprava premennej v cykle }
var
   a: integer;
begin
	for a := 10  to 20 do
	begin
		a := 10*2;
    	write('value of a: ', a)
	end
end.

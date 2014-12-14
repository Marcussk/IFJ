{ repeat until with nested block}
var 
	a : integer;
 	
begin
	a := 6;

	repeat
	begin
		begin
			begin
				write(a, ''#10'');
				a := a - 1
			end
		end
	end
	until (a = 4)
end.
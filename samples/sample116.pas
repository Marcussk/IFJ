{ Jednoduchy test na while }
var
    a : integer;
begin
    a := 5 ;
    while a > 0 do
    begin
        write(a);
	    a := a - 1    
         end;
    write('vysl:', a, ''#10'')
end.

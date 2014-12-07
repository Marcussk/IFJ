{ Jednoduchy test na if }
var
    a : integer;
	b : integer;
begin
    a := 5;
    if a > 0 then
    begin
        write('kladne', ''#10'')
    end
    else
    begin
        write('zaporne', ''#10'')
    end;
 	
	b := 5 - 10;
    if b > 0 then
    begin
        write('kladne', ''#10'')
    end
    else
    begin
        write('zaporne', ''#10'')
    end
end.

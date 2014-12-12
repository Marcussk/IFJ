{ find string concat test / nyan version }
var
	res : string;
	i : integer;
begin
res := 'nyan ';
i := 10;
while i >0 do
begin
res := res + res;

i := i -1
end;

write(res, ''#10'')

end.

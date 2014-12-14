{ repeat until, builtins, FUNEXP}
var 
	str : string;
 	sum : integer;
begin
	sum := 0;
	str := 'jihgfedcba';
	repeat
	begin
		str := copy(sort(str), 2, length(str));
		sum := sum + length(str);
		write(str, ''#10'')
	end
	until (str = '');
	write(sum)
end.
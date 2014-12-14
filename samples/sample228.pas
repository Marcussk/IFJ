{ repeat until without block, builtins, FUNEXP}
var 
	str : string;
 	sum : integer;
begin
	sum := 0;
	str := 'jihgfedcba';
	repeat
		str := copy(sort(str), 2, length(str));
		sum := sum + length(str);
		write(str, ''#10'')
	until (str = '');
	write(sum)
end.
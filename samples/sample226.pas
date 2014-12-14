{ repeat until with builtins}
var 
	str : string;
 	sum : integer;
begin
	sum := 0;
	str := 'abcdefghij';
	repeat
	begin
		sum := 	sum + length(copy(str, 1, length(str) - 1));
		str := copy(str, 1, length(str) - 1);
		write(str, ''#10'')
	end
	until (str = '');
	write(sum)
end.
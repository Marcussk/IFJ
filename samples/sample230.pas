{ builtins in condition }

var 
	half : integer;
	str  : string;

begin
	str := 'abcdefgh';
	while (length(str) > length('')) do
	begin
		str := copy(str, 1, length(str)-1);
		write(str, ''#10'')
	end
end.
{ builtins, sem_error}
var 
	str : string;

begin

	str := 'abcdefghij';
	
	if (length(str) > 1) then
	begin
		str := copy(str, 1, length(str/2));
	end
	else
	begin
	end
end.
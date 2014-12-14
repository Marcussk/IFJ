{ copy with concatenation }
var
	str : string;
	copied : string;
begin
	str := 'Hello World';
	copied := copy('Hello'+''#32''+'World', 1, length('Hello'+''#32''+'World'));
	write('Copied string is: ', copied)
end.
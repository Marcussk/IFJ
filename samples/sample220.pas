{ sort - empty source }
var
	str1 : string;
	str2 : string;
	found : integer;
begin
	str1 := '';
	str2 := 'text';
	
	found := find(str1, str2);
	write(found)
end.
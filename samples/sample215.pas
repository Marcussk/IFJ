{ length with escape }
var
	len : integer;
	str : string;

begin
	str := 'Hello''World'#10#33;
	len := length(str);
	write('Delka je: ', len)
end.
CREATE TABLE before (a sha256);
INSERT INTO before values('6386fe89e71228d4619087c61429f6409f59a51d652efa4ce77e653749a4725f');
INSERT INTO before values('ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff');
INSERT INTO before values('145545225d9f92af3e0a46fef4a0904d01d998a9e0e7c9ec1b98b56a251c3bcc');
INSERT INTO before values('0000000000000000000000000000000000000000000000000000000000000000');
CREATE TABLE after (a sha256);
COPY after
FROM PROGRAM 'psql -U postgres -c "COPY before TO STDOUT WITH (FORMAT binary)"            contrib_regression '
WITH (FORMAT binary);
SELECT * FROM after order by a;
DROP TABLE before;
DROP TABLE after;

CREATE TABLE before (a md5hash);
INSERT INTO before values('6386fe89e71228d4619087c61429f6409');
INSERT INTO before values('ffffffffffffffffffffffffffffffff');
INSERT INTO before values('00000000000000000000000000000000');
INSERT INTO before values('145545225d9f92af3e0a46fef4a0904d');
CREATE TABLE after (a md5hash);
COPY after
FROM PROGRAM 'psql -U postgres -c "COPY before TO STDOUT WITH (FORMAT binary)"            contrib_regression '
WITH (FORMAT binary);
SELECT * FROM after order by a;
DROP TABLE before;
DROP TABLE after;
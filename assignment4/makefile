key: keygen.c
	gcc -o keygen keygen.c
enc: otp_enc.c
	gcc -o otp_enc otp_enc.c
encd: otp_enc_d.c
	gcc -o otp_enc_d otp_enc_d.c
decd: otp_dec_d.c
	gcc -o otp_dec_d otp_dec_d.c
dec: otp_dec.c
	gcc -o otp_dec otp_dec.c 
all:
	gcc -o keygen keygen.c
	gcc -o server server.c
	gcc -o client client.c
clean:
	rm -rf otp_enc otp_enc_d otp_dec otp_dec_d

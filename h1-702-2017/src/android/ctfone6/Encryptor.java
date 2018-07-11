import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

import java.lang.StringBuilder;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.crypto.Cipher;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class Encryptor {
	public static String hexEncode(byte[] byteArray) {
		StringBuilder output = new StringBuilder();
		for (byte b : byteArray) {
			output.append(String.format("%02X", b));
        }
		return output.toString();
    }

	public static byte[] hexDecode(String s) {
		int len = s.length();
		byte[] data = new byte[len / 2];
		for (int i = 0; i < len; i += 2) {
			data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4)
								 + Character.digit(s.charAt(i+1), 16));
		}
		return data; }

    public static byte[] encrypt(String key, String initVector, byte[] value) {
        try {
            IvParameterSpec iv = new IvParameterSpec(initVector.getBytes("UTF-8"));
            SecretKeySpec skeySpec = new SecretKeySpec(key.getBytes("UTF-8"), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.ENCRYPT_MODE, skeySpec, iv);

            byte[] encrypted = cipher.doFinal(value);
            return encrypted;
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return null;
    }

    public static byte[] decrypt(String key, String initVector, byte[] value) {
        try {
            IvParameterSpec iv = new IvParameterSpec(initVector.getBytes("UTF-8"));
            SecretKeySpec skeySpec = new SecretKeySpec(key.getBytes("UTF-8"), "AES");

            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5PADDING");
            cipher.init(Cipher.DECRYPT_MODE, skeySpec, iv);

            byte[] encrypted = cipher.doFinal(value);
            return encrypted;
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        return null;
    }

    public static void main(String[] args) {
        if (args.length != 5) {
            System.out.println("Usage: <key> <iv> <input> <output>");
            System.exit(0);
        }

        String method = args[0];
        String key = args[1];
        String initVector = args[2];

        File input = new File(args[3]);
        File output = new File(args[4]);

        try {
            BufferedInputStream inputReader = new BufferedInputStream(new FileInputStream(input));
            BufferedOutputStream outputWriter = new BufferedOutputStream(new FileOutputStream(output));

            byte[] outputBytes;
            byte[] inputBytes = new byte[(int) input.length()];
            inputReader.read(inputBytes);
            if (method.equals("encrypt")) {
                outputBytes = encrypt(key, initVector, inputBytes);
            } else {
                outputBytes = decrypt(key, initVector, inputBytes);
            }
            outputWriter.write(outputBytes);

            inputReader.close();
            outputWriter.close();
        } catch (IOException io) {
            System.err.println("Error encrypting");
            io.printStackTrace();
        }
        System.out.println("Done!");
    }
}

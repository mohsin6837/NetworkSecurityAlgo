import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.math.BigInteger;
public class SHA1 {
    public static String getSha1(String input)
	{
		try {

			MessageDigest md = MessageDigest.getInstance("SHA1");
			byte[] messageDigest = md.digest(input.getBytes());
			BigInteger no = new BigInteger(1, messageDigest);
			String hashtext = no.toString(16);
			while (hashtext.length() < 32) {
				hashtext = "0" + hashtext;
			}
			return hashtext;
		}
		catch (NoSuchAlgorithmException e) {
			throw new RuntimeException(e);
		}
	}
    public static void main(String[] args) {
        String s = "HelloWorld";
		System.out.println("Your HashCode Generated by MD5 is: " + getSha1(s));
    }
}
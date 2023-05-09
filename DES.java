import java.security.*;
import javax.crypto.*;
import java.util.*;
public class DES {
    public static void main(String args[])
    {
        try
        {
            Cipher desCipher = Cipher.getInstance("DES");
            KeyGenerator keygen = KeyGenerator.getInstance("DES");
            SecureRandom randomSeed = new SecureRandom();
            keygen.init(randomSeed);
            Key deskey = keygen.generateKey();
            int mode = Cipher.ENCRYPT_MODE;
            desCipher.init(mode, deskey);
            Scanner console = new Scanner(System.in);
            System.out.println("Enter a text for encryption");
            String plainText = console.nextLine();
            String encryptedText = crypt(desCipher,plainText);
            System.out.println("The encrypted text is " +encryptedText);
            mode = Cipher.DECRYPT_MODE;
            desCipher.init(mode, deskey);
            String decryptedText = crypt(desCipher,encryptedText);
            System.out.println("The decrypted text is "+decryptedText);
            console.close();
        }
        catch(GeneralSecurityException e)
                {
                    System.out.println("Security Exception :"+ e.getMessage());
                }
    }
    
    public static String crypt(Cipher desCipher, String in) throws GeneralSecurityException
    {
        int inSize = desCipher.getBlockSize();
        byte[] inBytes = new byte[inSize];
        inBytes = in.getBytes();
        int outSize = desCipher.getOutputSize(inSize);
        byte[] outBytes = new byte[outSize];
        outBytes = desCipher.doFinal(inBytes);
        String out = new String(outBytes);
        return out;
                
    }
}

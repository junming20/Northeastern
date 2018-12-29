import java.io.*;
import java.util.*;

public class Ecommerce {
	public static Map<String, Buyer> buyerMap = new HashMap<> ();
	public static Map<String, Seller> sellerMap = new HashMap<> ();
	public static Set<Product> inventorySet = new HashSet<> ();
	
	public static void main(String[] args) throws FileNotFoundException {
		Parser parser = new Parser(buyerMap, sellerMap, inventorySet);
		while (parser.hasNextLine()) {
			parser.nextLine();
		}
	}
}

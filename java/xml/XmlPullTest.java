
public class XmlPullTest {

	static public void main(String[] argc) {

		// Read partner-provided list of excluded input devices
		XmlPullParser parser = null;
		// Environment.getRootDirectory() is a fancy way of saying ANDROID_ROOT or "/system".
		File confFile = new File(Environment.getRootDirectory(), EXCLUDED_DEVICES_PATH);
		FileReader confreader = null;
		try {
			confreader = new FileReader(confFile);
			parser = Xml.newPullParser();
			parser.setInput(confreader);
			XmlUtils.beginDocument(parser, "devices");

			while (true) {
				XmlUtils.nextElement(parser);
				if (!"device".equals(parser.getName())) {
					break;
				}
				String name = parser.getAttributeValue(null, "name");
				if (name != null) {       
					if (DEBUG) Slog.v(TAG, "addExcludedDevice " + name);
					addExcludedDevice(name);
				}
			}
		} catch (FileNotFoundException e) {
			// It's ok if the file does not exist.
		} catch (Exception e) { 
			Slog.e(TAG, "Exception while parsing '" + confFile.getAbsolutePath() + "'", e);
		} finally {
			try { if (confreader != null) confreader.close(); } catch (IOException e) { }

		}
	}

}

{
  "targets": [
    {
      "target_name": "DecryptMail",
      "sources": ["DecryptMailNode.cpp","opensslutil.cpp","smime.cpp","strutil.cpp"],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "D:\\Tools\\Include\\include\\openssl"
      ]
    }
  ]
}

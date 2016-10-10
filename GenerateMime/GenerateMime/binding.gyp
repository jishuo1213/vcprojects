{
  "targets": [
    {
      "target_name": "mime",
      "sources": [ "GenerateMimeNode.cpp", "jsoncpp.cpp", "mime.cpp" ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "D:\\Tools\\Include\\include\\"
      ]
    }
  ]
}
